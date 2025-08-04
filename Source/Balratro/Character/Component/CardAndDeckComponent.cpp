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
	// 나중에 DeckNumIndex ==> 카드가 덱에 추가된 순서를 csv에 넣어줘야함

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
				
				// 여기서 이제 
				// 1번)현재 패에 있는 배수 카드 계산,
				// 2번) 현재 손패에 남은 카드에서 스틸카드 계산
				// 3번)  조커 카드 계산
				// 할때까지 패 안뽑고 점수 계산하게 만들어야함

				UpdateCardInHand(_DeckCardStat);
				
				
				DrawCard(CardNum);
				// 점수 올리는 이벤트 후에 다시 초기화 해야되지만 일단 초기화 먼저하는코드 넣기
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



