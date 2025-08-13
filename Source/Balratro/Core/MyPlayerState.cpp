// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
	:RoundCount(0), EntiCount(0), UseHandCount(0), MaxChuckCount(3), UseChuckCount(0), CurrentScore(0),
	 MaxScore(0), MaxHandCount(4), MaxGold(4), Gold(4), CurrentShowChip(0), CurrentShowDrainage(0),
	 CurSortType(EHandInCardSortType::SORT_RANK)
{
}

void AMyPlayerState::ResetMyHandRankingInfo(const TMap<const FName, FHandRankingStat*>& InHandRanking)
{
	for (const auto& Info : InHandRanking)
	{
		UHandRanking_Info* MyInfo = NewObject<UHandRanking_Info>();
		MyInfo->Info.Level = Info.Value->Level;
		MyInfo->Info.Chip = Info.Value->Chip;
		MyInfo->Info.Drainage = Info.Value->Drainage;
		MyInfo->Info.UseNum = Info.Value->UseNum;
		MyInfo->Info.IncreaseChip = Info.Value->IncreaseChip;
		MyInfo->Info.IncreaseDrainage = Info.Value->IncreaseDrainage;

		MyInfo->_Name = Info.Key;
		MyHandRankingInfo.Add(MyInfo);
	}
}

void AMyPlayerState::ResetDeckStatTable(const TArray<FDeckCardStat*>& InHandRanking)
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

		Deck_Stat.Add(StatInfo);
	}

}

void AMyPlayerState::SetCurCalculatorCardInHands(TArray<FDeckCardStat>& InValue)
{
	CurCalculatorCardInHands.Empty();
	CurCalculatorCardInHands = InValue; 

	if (InValue.Num() == 0)
	{
		SetCurHandCard_Type(EPokerHand::NONE);
	}
}

void AMyPlayerState::ReStart()
{

}

void AMyPlayerState::SetNextRound()
{
	SetRoundCount(RoundCount + 1);
	SetUseHandCount(0);
	SetUseChuckCount(0);
	SetCurrentScore(0);
	SetCardInHand(0);
	SetCardInDeckNum(Deck_Stat.Num());
	SetCurrentShowChip(0);
	SetCurrentShowDrainage(0);
	SetCurrentRoundBlindGrade(0);
	SetCurHandCard_Type(EPokerHand::NONE);
}