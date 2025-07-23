#include "Character/Component/CardAndDeckComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"


#include "Singleton/BBGameSingleton.h"
#include "Core/MyPlayerState.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"


// 142 /190

void UCardAndDeckComponent::BeginPlay()
{
	Super::BeginPlay();

	InitDeck();
}

void UCardAndDeckComponent::UpdateCardInHand(int32 _invalue)
{
	auto VM = GetVMCardDeck();
}

void UCardAndDeckComponent::UpdateCardInDeck()
{
	auto VM = GetVMCardDeck();
	auto PS = GetPlayerState();
	
	VM->SetDeckNum(PS->GetCardInDeck());
}

void UCardAndDeckComponent::ShuffleDeck()
{
	auto PS = GetPlayerState();

	auto& MyDeckStatTable = PS->GetDeckCardStatTableModify();

	FDateTime Now = FDateTime::UtcNow();
	int64 Milliseconds = Now.ToUnixTimestamp() * 1000 + Now.GetMillisecond();
	int32 Seed = static_cast<int32>(Milliseconds & 0xFFFFFFFF); // int32로 제한

	// 무작위 시드 설정
	FMath::RandInit(Seed);

	int32 NumElements = MyDeckStatTable.Num();

	for (int32 i = NumElements - 1; i > 0; --i)
	{
		// 0부터 i까지의 무작위 인덱스 선택
		int32 RandomIndex = FMath::RandRange(0, i);

		// 현재 요소와 무작위로 선택된 요소를 교환
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
	auto& MyDeckStatTable = PS->GetDeckCardStatTable();
	
	for (int i = CurIndex; i < CurIndex + DrawCardNum; ++i)
	{	
		if (!MyDeckStatTable[i].CardSprite.IsValid())
		{
			MyDeckStatTable[i].CardSprite.LoadSynchronous();
		}
	
		PS->AddHandInCard(MyDeckStatTable[i]);
	}

	SortHandInCard(PS->GetCurSortType());
	VM->SetCurrentHandInCards(PS->GetCurrentHandInCards());
	
	PS->SetCardInDeck(PS->GetCardInDeck() - DrawCardNum);
	VM->SetDeckNum(PS->GetCardInDeck());
	CurIndex += DrawCardNum;
}

void UCardAndDeckComponent::UpdateChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();

	int32 CurChuckCnt = PS->GetUseChuckCount();

	if (PS->GetMaxChuckCount() < CurChuckCnt + 1)
	{
		return;
	}
	
	PS->SetUseChuckCount(++CurChuckCnt);
	
	auto CurHandInCard = PS->GetCurrentHandInCardsModify();
	CurHandInCard.RemoveAll([&](UHandInCard_Info* HandCard)
		{
			if (!HandCard) return false;

			// DeckCardStats 배열에 HandCard->Info 와 같은 항목이 하나라도 있으면 true
			return _DeckCardStat.ContainsByPredicate([&](const FDeckCardStat& Stat)
				{
					return Stat == HandCard->Info;
				});
		});
	// 나중에 DeckNumIndex ==> 카드가 덱에 추가된 순서를 csv에 넣어줘야함


	PS->SetCurrentHandInCards(CurHandInCard);
	VM->SetCurrentHandInCards(PS->GetCurrentHandInCards());

	DrawCard(CardNum);
}

void UCardAndDeckComponent::SetHandPlay(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{

}

void UCardAndDeckComponent::InitDeck()
{
	auto& Sigleton = UBBGameSingleton::Get();
	auto PS = GetPlayerState();

	auto DataTable = Sigleton.GetDeckCardStatTable();

	PS->ResetDeckCardStatTable(DataTable);

	PS->SetCardInDeck(DataTable.Num());

	// Test
	{
		UpdateCardInDeck();
		ShuffleDeck();
		DrawCard(6);

		const auto VM = GetVMCardDeck();
		VM->OnSortTypeChange.AddUObject(this, &UCardAndDeckComponent::SortHandInCard);
		VM->OnUseChuck.AddUObject(this, &UCardAndDeckComponent::UpdateChuck);
		VM->OnUseHandPlay.AddUObject(this, &UCardAndDeckComponent::SetHandPlay);
	}
}

void UCardAndDeckComponent::SortHandInCard(const EHandInCardSortType& InType)
{
	auto PS = GetPlayerState();
	auto& CurHandInCard = PS->GetCurrentHandInCardsModify();
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

	VM->SetCurrentHandInCards(PS->GetCurrentHandInCards());
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

AMyPlayerState* UCardAndDeckComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}



