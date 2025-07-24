#include "Character/Component/HandRankingComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "Singleton/BBGameSingleton.h"
#include  "Algo/AnyOf.h"



// Called when the game starts
void UHandRankingComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerState = GetPlayerState();
	auto VM = GetVMHandRanking();
	auto& Sigleton = UBBGameSingleton::Get();

	FHandRankingStat* RoyalFlushHandStat = Sigleton.GetHandRankingStat(FName(TEXT("RoyalFlush")));
	VM->AddHandRankingNum(FName(TEXT("RoyalFlush")),*RoyalFlushHandStat);

	FHandRankingStat* FlushandStat = Sigleton.GetHandRankingStat(FName(TEXT("Flush")));
	VM->AddHandRankingNum(FName(TEXT("Flush")), *FlushandStat);


    const auto VM2 = GetVMCardDeck();
    VM2->OnHandRankName.AddUObject(this, &UHandRankingComponent::SetHandRankName);

}

UVM_HandRankingCount* UHandRankingComponent::GetVMHandRanking()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_HandRankingCount");
	Context.ContextClass = UVM_HandRankingCount::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_HandRankingCount>(Found);
}

UVM_CardDeck* UHandRankingComponent::GetVMCardDeck()
{
    const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

    FMVVMViewModelContext Context;
    Context.ContextName = TEXT("VM_CardDeck");
    Context.ContextClass = UVM_CardDeck::StaticClass();

    const auto Found = VMCollection->FindViewModelInstance(Context);
    return Cast<UVM_CardDeck>(Found);
}

UVM_PlayerInfo* UHandRankingComponent::GetVMPlayerInfo()
{
    const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

    FMVVMViewModelContext Context;
    Context.ContextName = TEXT("VM_PlayerInfo");
    Context.ContextClass = UVM_PlayerInfo::StaticClass();

    const auto Found = VMCollection->FindViewModelInstance(Context);
    return Cast<UVM_PlayerInfo>(Found);
}

AMyPlayerState* UHandRankingComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;	
}

EPokerHand UHandRankingComponent::CalCulatorHandRanking(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
    if (CardNum == 0)
        return EPokerHand::NONE;
    
    auto PlayerInfoVM = GetVMPlayerInfo();
    bool RoyalStraight = false;
	bool Straight = false;
	bool Flush = false;

	TArray<int> RankArray, SuitArray;

	TMap<int32, int32> RankCounts;
	TMap<int32, int32> SuitCounts;
	TSet<int32> UniqueRanks;
	TArray<int32> SortedRanks;

    // Test
    //CalculatorBaseScore(EPokerHand::FIVE_CARD, _DeckCardStat);

	for (int i = 0; i < CardNum; ++i)
	{
		RankCounts.FindOrAdd(_DeckCardStat[i].RankGrade)++;
		SuitCounts.FindOrAdd(_DeckCardStat[i].SuitGrade)++;
		UniqueRanks.Add(_DeckCardStat[i].RankGrade);
	}

	SortedRanks = UniqueRanks.Array();
	SortedRanks.Sort([](int32 A, int32 B) { return A < B; });

	bool bIsStraight = false;
	if (SortedRanks.Num() == 5)
	{
		bool bStraight = true;
		for (int32 i = 1; i < 5; ++i)
		{
			if (SortedRanks[i] != SortedRanks[i - 1] + 1)
			{
				bStraight = false;
				break;
			}
		}

		// A-5-4-3-2 (A는 1, 2는 13): 특수 스트레이트 처리
		bool bAceLowStraight = SortedRanks.Contains(1) &&
			SortedRanks.Contains(13) &&
			SortedRanks.Contains(12) &&
			SortedRanks.Contains(11) &&
			SortedRanks.Contains(10);

		bIsStraight = bStraight || bAceLowStraight;
	}

    // 플러시 확인
    bool bIsFlush = false;
    for (const auto& Pair : SuitCounts)
    {
        if (Pair.Value == 5)
        {
            bIsFlush = true;
            break;
        }
    }

    if (bIsStraight && bIsFlush)
    {
        if (SortedRanks.Contains(1) && SortedRanks.Contains(2) && SortedRanks.Contains(3) &&
            SortedRanks.Contains(4) && SortedRanks.Contains(5))
        {
            return EPokerHand::ROYAL_FLUSH;
        }
        return EPokerHand::STRAIGHT_FLUSH;
    }

    if (Algo::AnyOf(RankCounts, [](const TPair<int32, int32>& Pair)
        {
            return Pair.Value == 4;
        }))
    {
        return EPokerHand::FOUR_CARD;
    }

    bool bThree = false, bTwo = false;
    for (const auto& Pair : RankCounts)
    {
        if (Pair.Value == 3) 
            bThree = true;
        else if (Pair.Value == 2) 
            bTwo = true;
    }

    if (bThree && bTwo)
    {
        return EPokerHand::FULL_HOUSE;
    }

    if (bIsFlush)
    {
        return EPokerHand::FLUSH;
    }

    if (bIsStraight)
    {
        return EPokerHand::STRAIGHT;
    }

    if (bThree)
    {
        return EPokerHand::TRIPLE;
    }

    int32 PairCount = 0;
    for (const auto& Pair : RankCounts)
    {
        if (Pair.Value == 2)
        {
            PairCount++;
        }
    }

    if (PairCount == 2)
    {
        return EPokerHand::TWO_PAIR;
    }
    if (PairCount == 1)
    {
        return EPokerHand::ONE_PAIR;
    }

    return EPokerHand::HIGH_CARD;
}

void UHandRankingComponent::CalculatorBaseScore(EPokerHand HandRankingType, TArray<FDeckCardStat>& _DeckCardStat)
{
    auto PS = GetPlayerState();
    auto& Sigleton = UBBGameSingleton::Get();
    auto VMPlayerInfo = GetVMPlayerInfo();
    int CurrentChip = 0, CurrentDrainage = 0;

  /*  for (auto& CurChip : _DeckCardStat)
	{
		CurrentChip += (CurChip.BaseChip);
		if (CurChip.EnforceType == EnforceStatType::CHIP_PLUS)
			CurrentChip += 30;

		if (CurChip.EnforceType == EnforceStatType::DRAINAGE)
			CurrentDrainage += 4;
	}*/

    FHandRankingStat* RoyalFlushHandStat = Sigleton.GetHandRankingStat(FName(TEXT("RoyalFlush")));
    int32 Score = RoyalFlushHandStat->Chip * RoyalFlushHandStat->Drainage;
    PS->SetScore(Score);
    VMPlayerInfo->SetScroe(PS->GetScore());
    


    switch (HandRankingType)
    {
    case EPokerHand::HIGH_CARD:
        
        break;
    case EPokerHand::ONE_PAIR:
        break;
    case EPokerHand::TWO_PAIR:
        break;
    case EPokerHand::TRIPLE:
        break;
    case EPokerHand::STRAIGHT:
        break;
    case EPokerHand::FLUSH:
        break;
    case EPokerHand::FULL_HOUSE:
        break;
    case EPokerHand::FOUR_CARD:
        break;
    case EPokerHand::STRAIGHT_FLUSH:
        break;
    case EPokerHand::ROYAL_FLUSH:
        break;
    case EPokerHand::FIVE_CARD:
        break;
    default:
        break;
    }

}

void UHandRankingComponent::SetHandRankName(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
    auto VM_PlayerInfo = GetVMPlayerInfo();
    
    EPokerHand CurHandType = CalCulatorHandRanking(CardNum, _DeckCardStat);
   
    FName Name;
    switch (CurHandType)
    {
    case EPokerHand::NONE:
        Name = "-";
        break;
    case EPokerHand::HIGH_CARD:
        Name = "HIGH CARD";
        break;
    case EPokerHand::ONE_PAIR:
        Name = "ONE PAIR";
        break;
    case EPokerHand::TWO_PAIR:
        Name = "TWO PAIR";
        break;
    case EPokerHand::TRIPLE:
        Name = "TRIPLE";
        break;
    case EPokerHand::STRAIGHT:
        Name = "STRAIGHT";
        break;
    case EPokerHand::FLUSH:
        Name = "FLUSH";
        break;
    case EPokerHand::FULL_HOUSE:
        Name = "FULL HOUSE";
        break;
    case EPokerHand::FOUR_CARD:
        Name = "FOUR CARD";
        break;
    case EPokerHand::STRAIGHT_FLUSH:
        Name = "STRAIGHT FLUSH";
        break;
    case EPokerHand::ROYAL_FLUSH:
        Name = "ROYAL FLUSH";
        break;
    case EPokerHand::FIVE_CARD:
        Name = "FIVE CARD";
        break;
    default:
        break;
    }
    VM_PlayerInfo->SetHandName(Name);

    auto VM_CardDeck = GetVMCardDeck();
    VM_CardDeck->SetIsUpCardExist(false);

}
