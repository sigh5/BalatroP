// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MyPlayerState.h"
#include "GameData/BoucherStat.h"	
#include "GameData/Utills.h"

AMyPlayerState::AMyPlayerState()
	:RoundCount(0), EntiCount(0), UseHandCount(0), MaxChuckCount(3), UseChuckCount(0), CurrentScore(0),
	 MaxScore(0), MaxHandCount(1), MaxGold(4), Gold(4), CurrentShowChip(0), CurrentShowDrainage(0),
	 CurSortType(EHandInCardSortType::SORT_RANK)
{
	FRandomUtils::Init();
}

void AMyPlayerState::ResetMyHandRankingInfo(const TMap<const FName, FHandRankingStat*>& InHandRanking)
{
	MyHandRankingInfo.Empty();

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
	Deck_Stat.Empty();

	for (auto& Info : InHandRanking)
	{
		UHandInCard_Info* CardInfo = NewObject<UHandInCard_Info>();
		CardInfo->Info.Name = Info->Name;
		CardInfo->Info.BaseChip = Info->BaseChip;
		CardInfo->Info.SealType = Info->SealType;
		CardInfo->Info.EnforceType = Info->EnforceType; // EnforceStatType
		CardInfo->Info.GhostCardType = Info->GhostCardType;
		CardInfo->Info.RankGrade = Info->RankGrade;
		CardInfo->Info.SuitGrade = Info->SuitGrade;
		CardInfo->Info.CardSprite = Info->CardSprite;

		Deck_Stat.Add(CardInfo);
	}
}

void AMyPlayerState::SetCurCalculatorCardInHands(TArray<class UHandInCard_Info*>& InValue)
{
	CurCalculatorCardInHands.Empty();
	CurCalculatorCardInHands = InValue; 

	if (InValue.Num() == 0)
	{
		SetCurHandCard_Type(EPokerHand::NONE);
	}
}

void AMyPlayerState::SetCurCalculatorCardInHands0(TArray<UHandInCard_Info*>& InValue, bool bPlay)
{
	CurCalculatorCardInHands.Empty();
	
	for (auto CurHandsCard : CurrentAllHands)
	{
		for (auto& CardStat : InValue)
		{
			if (CurHandsCard->Info == CardStat->Info)
			{
				if (bPlay)
				{
					CurHandsCard->Info.UseNum++;
				}

				CurCalculatorCardInHands.Add(CurHandsCard);
			
				break;
			}
		}
	}

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
	
	CurrentAllHands.Empty();
}

void AMyPlayerState::AddBoucherType(FBoucherInfo& _InValue)
{
	CurBoucherInfo.Add(_InValue);
	
	EBoucherType CurType = _InValue.Type;

	UE_LOG(LogTemp, Warning, TEXT("MaxChuckCount : %d"), MaxChuckCount);
	UE_LOG(LogTemp, Warning, TEXT("MaxHandCount : %d"), MaxHandCount);

	switch (CurType)
	{
	case EBoucherType::NONE:
		break;
	case EBoucherType::BoucherType_HAND_PULS:
		break;
	case EBoucherType::BoucherType_BOSS_REROLL:
		break;
	case EBoucherType::BoucherType_ENTI_MINUS:
		break;
	case EBoucherType::BoucherType_INTEREST_50:
		break;
	case EBoucherType::BoucherType_CHUCK:
		SetMaxChuckCount(MaxChuckCount + 1);
		SetUseChuckCount(0);
		break;
	case EBoucherType::BoucherType_HAND:
		SetMaxHandCount(MaxHandCount + 1);
		SetUseHandCount(0);
		break;
	case EBoucherType::BoucherType_ORB:
		break;
	case EBoucherType::BoucherType_INVENTORY:
		break;
	case EBoucherType::BoucherType_COST_DOWN:
		break;
	case EBoucherType::BoucherType_INVENTORY_ORB:
		break;
	default:
		break;
	}
}

void AMyPlayerState::ResetInfos()
{
	FRandomUtils::Init();
	
	SetRoundCount(0);
	SetEntiCount(0);
	SetGold(4);
	SetMaxGold(4);
	SetUseHandCount(0);
	SetMaxHandCount(1); // 나중에 4로 바꾸기
	SetMaxChuckCount(3);
	SetUseChuckCount(0);
	SetCurrentScore(0);
	SetCardInHand(0);
	SetMaxScore(0);
	SetCurrentShowChip(0);
	SetCurrentShowDrainage(0);
	SetCurrentRoundBlindGrade(0);
	SetCurHandCard_Type(EPokerHand::NONE);

	MyHandRankingInfo.Empty();
	CurrentAllHands.Empty();
	CurSelcetPackType = nullptr;
	Deck_Stat.Empty();
	CurCalculatorCardInHands.Empty();
	CurRestCardInHands.Empty();
	CurJokerCardInfo.Empty();
	CurTaroStatTable.Empty();

	SetPlayerState(EPlayerStateType::RESET_GAME);
}
