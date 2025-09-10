// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/CalculatorHandRankingComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "Interface/JokerCalculatorInterface.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"
#include  "Algo/AnyOf.h"

void UCalculatorHandRankingComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PS = GetPlayerState();
	const auto VM_CardDeck = GetVMCardDeck();
	
	VM_CardDeck->OnCurPlayHands.AddUObject(this, &UCalculatorHandRankingComponent::SetHandRankName);
	PS->OnSelectNextScene.AddUObject(this, &UCalculatorHandRankingComponent::ResetComponentInfo);

	TArray<UHandInCard_Info*> none; // 초기값 세팅을 위해
	SetHandRankName(0, none);
}

int32 UCalculatorHandRankingComponent::CalCulatorHandRanking(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat)
{
	auto PS = GetPlayerState();
	PS->SetHandPlayFlag(true);
	
	SetCalRestCardInHands(_DeckCardStat); 
	
	PS->UseBossSkill(); // 점수 계산 전 보스 능력 확인

	SetStillCards();    // 스틸카드 계산

	SetHandRankingType(CardNum, _DeckCardStat);
	
	return ResultScore();
}

void UCalculatorHandRankingComponent::SetHandRankName(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat)
{
	auto PS = GetPlayerState();
    auto VM_CardDeck = GetVMCardDeck();

	PS->SetAllCurSelectCard(_DeckCardStat);
	
	if (PS->GetPlayerState() != EPlayerStateType::ITEM_SELECT)
	{
		SetHandRankingType(CardNum, _DeckCardStat);
	}
	
	if (CardNum == 5)  // 내가 공격할 수 있는 최대의 패
    {
        VM_CardDeck->SetIsSelectedMax(true);
    }
    else
    {
        VM_CardDeck->SetIsSelectedMax(false);
    }

    VM_CardDeck->SetIsUpCardExist(false);
}

void UCalculatorHandRankingComponent::SetHandRankingType(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	TArray<UHandInCard_Info*> CurCalCulatorCards;

	if (CardNum == 0)
	{
		PS->SetCurHandCard_Type(EPokerHand::NONE);
		return;
	}

	TArray<int> RankArray, SuitArray;
	TMap<int32, int32> RankCounts;
	TMap<int32, int32> SuitCounts;
	TSet<int32> UniqueRanks;
	TArray<int32> SortedRanks;

	for (int i = 0; i < CardNum; ++i)
	{
		RankCounts.FindOrAdd(_DeckCardStat[i]->Info.RankGrade)++;
		SuitCounts.FindOrAdd(_DeckCardStat[i]->Info.SuitGrade)++;
		UniqueRanks.Add(_DeckCardStat[i]->Info.RankGrade);
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
			PS->SetCurHandCard_Type(EPokerHand::ROYAL_FLUSH);
			return;
		}
		PS->SetCurHandCard_Type(EPokerHand::STRAIGHT_FLUSH);
		return;
	}

	if (bIsFlush)
	{
		PS->SetCurCalculatorCardInHands(_DeckCardStat);
		PS->SetCurHandCard_Type(EPokerHand::FLUSH);
		return;
	}

	if (bIsStraight)
	{
		PS->SetCurCalculatorCardInHands(_DeckCardStat);
		PS->SetCurHandCard_Type(EPokerHand::STRAIGHT);
		return;
	}

	TSet<int32> SameRankSet;
	if (Algo::AnyOf(RankCounts, [&SameRankSet](const TPair<int32, int32>& Pair)
		{
			SameRankSet.Add(Pair.Key);
			return Pair.Value == 4; })
		)
	{
		check(SameRankSet.Num() != 0)
			for (auto Card : _DeckCardStat)
			{
				if (Card->Info.RankGrade == *SameRankSet.begin())
				{
					CurCalCulatorCards.Add(Card);
				}
			}
		PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
		PS->SetCurHandCard_Type(EPokerHand::FOUR_CARD);
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
		PS->SetCurHandCard_Type(EPokerHand::FULL_HOUSE);
		return;
	}


	if (bThree)
	{
		check(SameRankSet.Num() != 0)

			for (auto Card : _DeckCardStat)
			{
				if (Card->Info.RankGrade == *SameRankSet.begin())
				{
					CurCalCulatorCards.Add(Card);
				}
			}
		PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
		PS->SetCurHandCard_Type(EPokerHand::TRIPLE);
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
				if (Card->Info.RankGrade == CurPairNum)
				{
					CurCalCulatorCards.Add(Card);
				}
			}
		}
		PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
		PS->SetCurHandCard_Type(EPokerHand::TWO_PAIR);
		return;
	}
	if (PairCount == 1)
	{
		check(SameRankSet.Num() != 0);

		for (auto Card : _DeckCardStat)
		{
			if (Card->Info.RankGrade == *SameRankSet.begin())
			{
				CurCalCulatorCards.Add(Card);
			}
		}
		PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
		PS->SetCurHandCard_Type(EPokerHand::ONE_PAIR);
		return;
	}

	for (auto Card : _DeckCardStat)
	{
		if (Card->Info.RankGrade == SortedRanks[0])
		{
			CurCalCulatorCards.Add(Card);
			break;
		}
	}
	PS->SetCurCalculatorCardInHands(CurCalCulatorCards);
	PS->SetCurHandCard_Type(EPokerHand::HIGH_CARD);
}

void UCalculatorHandRankingComponent::Calculator_StillCard(OUT int32& CurChip, OUT float& CurDriange)
{
	if (CurStillCard.Num() == 0)
		return;

	auto PS = GetPlayerState();
	auto VM_CardDeck = GetVMCardDeck();

	bool GoadFlag = (PS->GetPlayerState() == EPlayerStateType::BOSS_BLIND &&
		PS->GetCurBossType().Value == EBossType::GOAD);

	for (auto& SteelCard : CurStillCard)
	{
		if (GoadFlag && SteelCard->Info.RankGrade == 1)
			continue;

		CurDriange *= 1.5f;
	}
}

void UCalculatorHandRankingComponent::Calculator_Joker(OUT int32& CurChip, OUT float& CurDriange)
{
	for (UActorComponent* Comp : GetOwner()->GetComponents())
	{
		if (Comp->GetClass()->ImplementsInterface(UJokerCalculatorInterface::StaticClass()))
		{
			IJokerCalculatorInterface* InterfacePtr = Cast<IJokerCalculatorInterface>(Comp);
			InterfacePtr->CalculatorJokerSkill(CurChip, CurDriange);
			break;
		}
	}
}

int32 UCalculatorHandRankingComponent::ResultScore()
{
	auto PS = GetPlayerState();
	auto CurPlayCards = PS->GetCurCalculatorCardInHands();

	int32 Score = 0;

	if (PS->GetHandPlayFlag() == true)
	{	
		int32 ShowChip = PS->GetCurrentShowChip();
		float ShowDrainage = static_cast<float>(PS->GetCurrentShowDrainage());

		bool GoadFlag = (PS->GetCurBossType().Value == EBossType::GOAD && 
						 PS->GetPlayerState() == EPlayerStateType::BOSS_BLIND);

		for (auto cardInfo : CurPlayCards)
		{
			if (GoadFlag && cardInfo->Info.SuitGrade == 1)
			{
				continue;
			}

			ShowChip += cardInfo->Info.BaseChip;
			if (cardInfo->Info.EnforceType == ETaroSkillType::DRAINAGE)
			{
				ShowDrainage += 4;
			}
			else if (cardInfo->Info.EnforceType == ETaroSkillType::CHIP_PLUS)
			{
				ShowChip += 30;
			}
		}

		Calculator_StillCard(ShowChip, ShowDrainage);  // 여기까지 손패에서 계산
		Calculator_Joker(ShowChip, ShowDrainage);

		Score = static_cast<int>(static_cast<float>(ShowChip) * ShowDrainage);
	}
	
	return Score;
}

void UCalculatorHandRankingComponent::ResetComponentInfo(EPlayerStateType InValue)
{
	if (InValue == EPlayerStateType::RESET_GAME)
	{
		TArray<UHandInCard_Info*> none; // 초기값 세팅을 위해
		SetHandRankName(0, none);
	}
}

void UCalculatorHandRankingComponent::SetCalRestCardInHands(TArray<UHandInCard_Info*>& _CurPlayCards)
{
	auto PS = GetPlayerState();
	auto CurrentAllHands = PS->GetCurrentAllHands();

	TArray<UHandInCard_Info*> RestCards;

	for (auto* Card : CurrentAllHands)
	{
		if (!_CurPlayCards.Contains(Card))
		{
			RestCards.Add(Card);
		}
	}
	
	PS->SetRestCardInHands(RestCards);

}

void UCalculatorHandRankingComponent::SetStillCards()
{
	CurStillCard.Empty();

	auto PS = GetPlayerState();
	auto CurrentRestHands = PS->GetRestCardInHands();

	for (auto* Card : CurrentRestHands)
	{
		if (Card->Info.EnforceType == ETaroSkillType::STEEL)
		{
			CurStillCard.Add(Card);
		}
	}
}

bool UCalculatorHandRankingComponent::IsStraight(TArray<int32>& SortedRanks)
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

bool UCalculatorHandRankingComponent::IsFlush(TMap<int32, int32>& SuitCounts)
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

UVM_CardDeck* UCalculatorHandRankingComponent::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}

AMyPlayerState* UCalculatorHandRankingComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}
