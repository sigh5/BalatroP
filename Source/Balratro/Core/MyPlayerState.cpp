// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
	:RoundCount(0), EntiCount(0), UseHandCount(0), MaxChuckCount(3), UseChuckCount(0), CurrentScore(0)
	, MaxScore(0), CurSortType(EHandInCardSortType::SORT_RANK)
{
	MaxGold = Gold = 4;
	MaxHandCount = 4;
}

void AMyPlayerState::ResetDeckCardStatTable(const TArray<FDeckCardStat*>& InHandRanking)
{
	FDeckCardStat StatInfo;

	for (auto& Info : InHandRanking)
	{
		StatInfo.Name = Info->Name;
		StatInfo.BaseChip = Info->BaseChip;
		StatInfo.SealType = Info->SealType;
		StatInfo.EnforceType = Info->EnforceType;
		StatInfo.GhostCardType = Info->GhostCardType;
		StatInfo.RankGrade = Info->RankGrade;
		StatInfo.SuitGrade = Info->SuitGrade;
		StatInfo.CardSprite = Info->CardSprite;

		MyDeckCardStat.Add(StatInfo);
	}

}

void AMyPlayerState::SetCurCalculatorCardInHands(TArray<FDeckCardStat>& InValue)
{
	CurCalculatorCardInHands.Empty();


	CurCalculatorCardInHands = InValue; 

	bool c = false;
}
