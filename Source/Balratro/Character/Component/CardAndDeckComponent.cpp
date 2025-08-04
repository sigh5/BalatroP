#include "Character/Component/CardAndDeckComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"


#include "Singleton/BBGameSingleton.h"
#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_CardDeck.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"

#include "GameData/HandRankingStat.h"
#include "Interface/CalculatorScoreInterface.h"

void UCardAndDeckComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto VM = GetVMCardDeck();
	auto	PS = GetPlayerState();
	PS->OnCardBattleScene.AddUObject(this, &UCardAndDeckComponent::SetVisibleCardDeckView);

	VM->OnSortTypeChange.AddUObject(this, &UCardAndDeckComponent::SortHandInCard);
	VM->OnUseChuck.AddUObject(this, &UCardAndDeckComponent::UpdateChuck);
	VM->OnUseHandPlay.AddUObject(this, &UCardAndDeckComponent::UpdateHandPlay);
}

void UCardAndDeckComponent::UpdateCardInHand(TArray<FDeckCardStat>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	auto CurHandInCard = PS->GetCurrentAllHandsModify();
	CurHandInCard.RemoveAll([&](UHandInCard_Info* HandCard)
		{
			if (!HandCard)
				return false;

			return _DeckCardStat.ContainsByPredicate([&](const FDeckCardStat& Stat)
				{
					return Stat == HandCard->Info;
				});
		});
	// ���߿� DeckNumIndex ==> ī�尡 ���� �߰��� ������ csv�� �־������

	PS->SetCurrentAllHands(CurHandInCard);

	TArray<FDeckCardStat> Empty;
	PS->SetCurCalculatorCardInHands(Empty);
	
}

void UCardAndDeckComponent::SetVisibleCardDeckView(EPlayerStateType InValue)
{
	if (InValue == EPlayerStateType::SMALL_BLIND || InValue == EPlayerStateType::BIG_BLIND
		|| InValue == EPlayerStateType::SMALL_BLIND)
	{
		auto VM_MainMenu = GetVMMainWidget();
		FName CurName = "CadDeckView";
		VM_MainMenu->SetCurWidgetName(CurName);
		InitDeck();
	}
}

void UCardAndDeckComponent::ShuffleDeck()
{
	auto PS = GetPlayerState();

	auto& MyDeckStatTable = PS->GetDeckStatTableModify();

	FDateTime Now = FDateTime::UtcNow();
	int64 Milliseconds = Now.ToUnixTimestamp() * 1000 + Now.GetMillisecond();
	int32 Seed = static_cast<int32>(Milliseconds & 0xFFFFFFFF); 
	FMath::RandInit(Seed);

	int32 NumElements = MyDeckStatTable.Num();
	for (int32 i = NumElements - 1; i > 0; --i)
	{
		int32 RandomIndex = FMath::RandRange(0, i);
		if (i != RandomIndex)
		{
			MyDeckStatTable.Swap(i, RandomIndex);
		}
	}
}

void UCardAndDeckComponent::DrawCard(int32 DrawCardNum)
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();
	auto& MyDeckStatTable = PS->GetDeckStatTable();
	
	for (int i = CurDrawIndex; i < CurDrawIndex + DrawCardNum; ++i)
	{	
		if (!MyDeckStatTable[i].CardSprite.IsValid())
		{
			MyDeckStatTable[i].CardSprite.LoadSynchronous();
		}
	
		PS->AddHandInCard(MyDeckStatTable[i]);
	}
	SortHandInCard(PS->GetCurSortType());

	VM->SetCurrentAllHands(PS->GetCurrentAllHands());
	VM->SetIsSelectedMax(false);

	PS->SetCardInDeckNum(PS->GetCardInDeckNum() - DrawCardNum);
	CurDrawIndex += DrawCardNum;
}

void UCardAndDeckComponent::UpdateChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	int32 CurChuckCnt = PS->GetUseChuckCount();
	if (PS->GetMaxChuckCount() < CurChuckCnt + 1)
	{
		return;
	}
	
	PS->SetUseChuckCount(++CurChuckCnt);
	UpdateCardInHand(_DeckCardStat);
	DrawCard(CardNum);
}

void UCardAndDeckComponent::UpdateHandPlay(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
	auto PS = GetPlayerState();
	
	int32 MaxCount = PS->GetMaxHandCount();
	int32 UseCount = PS->GetUseHandCount();

	if (MaxCount < UseCount + 1)
		return;

	for (UActorComponent* Comp : GetOwner()->GetComponents())
	{
		if (Comp->GetClass()->ImplementsInterface(UCalculatorScoreInterface::StaticClass()))
		{
			ICalculatorScoreInterface* InterfacePtr = Cast<ICalculatorScoreInterface>(Comp);
			if (InterfacePtr)
			{
				InterfacePtr->CalCulatorHandRanking(CardNum,_DeckCardStat);
				
				// ���⼭ ���� 
				// 1��)���� �п� �ִ� ��� ī�� ���,
				// 2��) ���� ���п� ���� ī�忡�� ��ƿī�� ���
				// 3��)  ��Ŀ ī�� ���
				// �Ҷ����� �� �Ȼ̰� ���� ����ϰ� ��������

				UpdateCardInHand(_DeckCardStat);
				
				
				DrawCard(CardNum);
				// ���� �ø��� �̺�Ʈ �Ŀ� �ٽ� �ʱ�ȭ �ؾߵ����� �ϴ� �ʱ�ȭ �����ϴ��ڵ� �ֱ�
				break;
			}
		}
	}

	PS->SetUseHandCount(UseCount + 1);
}

void UCardAndDeckComponent::InitDeck()
{
	ShuffleDeck();
	DrawCard(8);
}

void UCardAndDeckComponent::SortHandInCard(const EHandInCardSortType& InType)
{
	auto PS = GetPlayerState();
	auto& CurHandInCard = PS->GetCurrentAllHandsModify();
	const auto VM = GetVMCardDeck();

	if (InType == EHandInCardSortType::SORT_RANK)
	{
		CurHandInCard.Sort([&](const auto& A, const auto& B)
			{
				if (A.Info.RankGrade == B.Info.RankGrade)
				{
					return A.Info.SuitGrade < B.Info.SuitGrade;
				}

				return A.Info.RankGrade < B.Info.RankGrade;
			});

	}
	else
	{
		CurHandInCard.Sort([&](const auto& A, const auto& B)
			{
				if (A.Info.SuitGrade == B.Info.SuitGrade)
				{
					return A.Info.RankGrade < B.Info.RankGrade;
				}

				return A.Info.SuitGrade < B.Info.SuitGrade;
			});
	}

	VM->SetCurrentAllHands(PS->GetCurrentAllHands());
}

UVM_CardDeck* UCardAndDeckComponent::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}

UVM_MainMenu* UCardAndDeckComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}


AMyPlayerState* UCardAndDeckComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}



