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

void UHandRankingComponent::CalCulatorHandRanking(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
    SetHandRankingType(CardNum, _DeckCardStat);



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
}

void UHandRankingComponent::SetHandRankName(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
    auto VM_PlayerInfo = GetVMPlayerInfo();
    auto PS = GetPlayerState();
    SetHandRankingType(CardNum, _DeckCardStat);
    
    EPokerHand CurHandType = PS->GetCardInDeckHandType();
   
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

void UHandRankingComponent::SetHandRankingType(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
    auto PS = GetPlayerState();
    TArray<FDeckCardStat> CurCalCulatorCards;

    if (CardNum == 0)
    {
        PS->SetCardInDeckHandType(EPokerHand::NONE);
        return;
    }
    auto PlayerInfoVM = GetVMPlayerInfo();
    
    TArray<int> RankArray, SuitArray;
    TMap<int32, int32> RankCounts;
    TMap<int32, int32> SuitCounts;
    TSet<int32> UniqueRanks;
    TArray<int32> SortedRanks;

    for (int i = 0; i < CardNum; ++i)
    {
        RankCounts.FindOrAdd(_DeckCardStat[i].RankGrade)++;
        SuitCounts.FindOrAdd(_DeckCardStat[i].SuitGrade)++;
        UniqueRanks.Add(_DeckCardStat[i].RankGrade);
    }

    SortedRanks = UniqueRanks.Array();
    SortedRanks.Sort([](int32 A, int32 B) { return A < B; });

    bool bIsStraight = IsStraight(SortedRanks);
    bool bIsFlush = IsFlush(SuitCounts);
  
    // 스트플 확인
    if (bIsStraight && bIsFlush)
    {
        PS->SetCurCalculatorCardInHands(_DeckCardStat);
        if (SortedRanks.Contains(1) && SortedRanks.Contains(2) && SortedRanks.Contains(3) &&
            SortedRanks.Contains(4) && SortedRanks.Contains(5))
        {
            PS->SetCardInDeckHandType(EPokerHand::ROYAL_FLUSH);
            return;
        }
        PS->SetCardInDeckHandType(EPokerHand::STRAIGHT_FLUSH);
        return;
    }

    if (bIsFlush)
    {
        PS->SetCurCalculatorCardInHands(_DeckCardStat);
        PS->SetCardInDeckHandType(EPokerHand::FLUSH);
        return;
    }

    if (bIsStraight)
    {
        PS->SetCurCalculatorCardInHands(_DeckCardStat);
        PS->SetCardInDeckHandType(EPokerHand::STRAIGHT);
        return;
    }

    TSet<int32> SameRankSet;
    if (Algo::AnyOf(RankCounts, [&SameRankSet](const TPair<int32, int32>& Pair)
        {
            SameRankSet.Add(Pair.Key);
            return Pair.Value == 4;})
        )
    {
        check(SameRankSet.Num() != 0)
        for (auto Card : _DeckCardStat)
        {
            if (Card.RankGrade == *SameRankSet.begin())
            {
                CurCalCulatorCards.Add(Card);
            }
        }
        PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
        PS->SetCardInDeckHandType(EPokerHand::FOUR_CARD);
        return;
    }

    bool bThree = false, bTwo = false;
    
    for (const auto& Pair : RankCounts)
    {
        if (Pair.Value == 3)
        {
            SameRankSet.Add(Pair.Key);
            bThree = true;
        }
        else if (Pair.Value == 2)
        {
            SameRankSet.Add(Pair.Key);
            bTwo = true;
        }
    }

    if (bThree && bTwo)
    {
        PS->SetCurCalculatorCardInHands(_DeckCardStat);
        PS->SetCardInDeckHandType(EPokerHand::FULL_HOUSE);
        return;
    }


    if (bThree)
    {
        check(SameRankSet.Num() != 0)

        for (auto Card : _DeckCardStat)
        {
            if (Card.RankGrade == *SameRankSet.begin())
            {
                CurCalCulatorCards.Add(Card);
            }
        }
        PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
        PS->SetCardInDeckHandType(EPokerHand::TRIPLE);
        return;
    }

    SameRankSet.Empty();

    int32 PairCount = 0;
    for (const auto& Pair : RankCounts)
    {
        if (Pair.Value == 2)
        {
            SameRankSet.Add(Pair.Key);
            PairCount++;
        }
    }

    if (PairCount == 2)
    {
        for (const auto& CurPairNum : SameRankSet)
        {
            for (auto Card : _DeckCardStat)
            {
                if (Card.RankGrade == CurPairNum)
                {
                    CurCalCulatorCards.Add(Card);
                }
            }
        }
        PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
        PS->SetCardInDeckHandType(EPokerHand::TWO_PAIR);
        return;
    }
    if (PairCount == 1)
    {
        check(SameRankSet.Num() != 0);

		for (auto Card : _DeckCardStat)
		{
			if (Card.RankGrade == *SameRankSet.begin())
			{
				CurCalCulatorCards.Add(Card);
			}
		}
        PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
        PS->SetCardInDeckHandType(EPokerHand::ONE_PAIR);
        return;
    }

    for (auto Card : _DeckCardStat)
    {
        if (Card.RankGrade == SortedRanks[0])
        {
            CurCalCulatorCards.Add(Card);
            break;
        }
    }
    PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
    PS->SetCardInDeckHandType(EPokerHand::HIGH_CARD);
}

bool UHandRankingComponent::IsStraight(TArray<int32>& SortedRanks)
{
    bool bIsStraight = false;
    if (SortedRanks.Num() == 5) //나중에 조커 카드일때 수정필요
    {
        bool bStraight = true;
        for (int32 i = 1; i < 5; ++i) //나중에 조커 카드일때 수정필요
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

    return bIsStraight;
}

bool UHandRankingComponent::IsFlush(TMap<int32, int32>& SuitCounts)
{
    bool bIsFlush = false;

    for (const auto& Pair : SuitCounts)
    {
        if (Pair.Value == 5)
        {
            bIsFlush = true;
            break;
        }
    }

    return bIsFlush;
}
