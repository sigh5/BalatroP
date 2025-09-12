// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MyPlayerState.h"
#include "GameData/BoucherStat.h"	
#include "GameData/Utills.h"
#include "Singleton/BBGameSingleton.h"

AMyPlayerState::AMyPlayerState()
	:RoundCount(0), EntiCount(0), UseHandCount(0), MaxChuckCount(3), UseChuckCount(0), CurrentScore(0),
	 MaxScore(0), MaxHandCount(2), MaxGold(4), Gold(4), CurrentShowChip(0), CurrentShowDrainage(0),
	 CurSortType(EHandInCardSortType::SORT_RANK)
{
	FRandomUtils::Init();
}

void AMyPlayerState::ResetMyHandRankingInfo(const TMap<const FName, FHandRankingStat*>& InHandRanking)
{
	MyHandRankingInfo.Empty();

	EPokerHand PokerHandsType[12] = {
		EPokerHand::FlUSH_FIVE_CARD,
		EPokerHand::FIVE_CARD,
		EPokerHand::ROYAL_FLUSH,
		EPokerHand::FOUR_CARD,
		EPokerHand::FULL_HOUSE,
		EPokerHand::STRAIGHT_FLUSH,
		EPokerHand::FLUSH,
		EPokerHand::STRAIGHT,
		EPokerHand::TRIPLE,
		EPokerHand::TWO_PAIR,
		EPokerHand::ONE_PAIR,
		EPokerHand::HIGH_CARD,
	};

	int32 Index = 0;
	for (const auto& Info : InHandRanking)
	{
		UHandRanking_Info* MyInfo = NewObject<UHandRanking_Info>();
		MyInfo->Info.Level = Info.Value->Level;
		MyInfo->Info.Chip = Info.Value->Chip;
		MyInfo->Info.Drainage = Info.Value->Drainage;
		MyInfo->Info.UseNum = Info.Value->UseNum;
		
		if (Index == 11)// Test
		{
			MyInfo->Info.Level += 2;
			MyInfo->Info.UseNum = 3;
		}
		
		MyInfo->Info.IncreaseChip = Info.Value->IncreaseChip;
		MyInfo->Info.IncreaseDrainage = Info.Value->IncreaseDrainage;

		MyInfo->_Name = Info.Key;
		MyInfo->_Type = PokerHandsType[Index++];
		MyHandRankingInfo.Add(MyInfo);
	}
}

void AMyPlayerState::ResetDeckStatTable(const TArray<FDeckCardStat*>& InHandRanking)
{
	Deck_Stat.Empty();

	//Test
	auto Temp = InHandRanking[0];

	for (auto& Info : InHandRanking)
	{
		UHandInCard_Info* CardInfo = NewObject<UHandInCard_Info>();
		CardInfo->Info.Name =	Info->Name;
		CardInfo->Info.BaseChip = Info->BaseChip;
		CardInfo->Info.SealType = Info->SealType;
		CardInfo->Info.EnforceType = Info->EnforceType; // ETaroSkillType
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

void AMyPlayerState::SetAllCurSelectCard(TArray<UHandInCard_Info*>& InValue)
{
	CurSelectedAllCard.Empty();
	
	for (auto CurHandsCard : CurrentAllHands)
	{
		for (auto& CardStat : InValue)
		{
			if (CurHandsCard->Info == CardStat->Info)
			{
				CurSelectedAllCard.Add(CurHandsCard);
				break;
			}
		}
	}
}

void AMyPlayerState::DeleteCurrentJokerCard(const FJokerStat& Info)
{
	int32 Num = CurJokerCardInfo.Num();

	int32 Index = -1;
	for (int i = 0; i < Num; ++i)
	{
		if (Info == CurJokerCardInfo[i]->Info)
		{
			Index = i;
			break;
		}
	}

	if (Index != -1)
	{
		CurJokerCardInfo.RemoveAtSwap(Index);
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

	switch (CurType)
	{
	case EBoucherType::NONE:
		break;
	case EBoucherType::BoucherType_HAND_PULS:
		DrawCardNum += 1;
		break;
	case EBoucherType::BoucherType_BOSS_REROLL:
		
		break;
	case EBoucherType::BoucherType_ENTI_MINUS:
		SetEntiCount(EntiCount - 1);
		SetMaxHandCount(MaxHandCount - 1);
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
		HaveUpStoreNum += 1;
		break;

	case EBoucherType::BoucherType_COST_DOWN:
		IsHavePriceDownBoucher = true;
		break;
	case EBoucherType::BoucherType_INVENTORY_ORB:
		
		break;
	default:
		break;
	}
}

UHandRanking_Info* AMyPlayerState::MostUseHandRankingName()
{
	TArray<UHandRanking_Info*> HandRankingInfo = MyHandRankingInfo; // 복사
	HandRankingInfo.Sort([&](const UHandRanking_Info& a, const UHandRanking_Info& b)
		{
			return a.Info.UseNum > b.Info.UseNum;
		});

	return HandRankingInfo[0];
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

	SetHaveUpStoreNum(2);
	MaxHaveBoosterPackNum = 2;
	HaveUpStoreNum = 2;
	IsHavePriceDownBoucher = false;

	DrawCardNum = 8;
	RerollCost = 3;


	SetPlayerState(EPlayerStateType::RESET_GAME);
}

void AMyPlayerState::FindRankUpCard(OUT FDeckCardStat& CardStat)
{
	int32 CurRankGrade = CardStat.RankGrade;

	if (CurRankGrade - 1 == 0)
	{
		CurRankGrade = 13;
	}
	else
	{
		CurRankGrade -= 1;
	}

	auto OriginData = UBBGameSingleton::Get().GetDeckStatTable();

	FDeckCardStat FindData; 

	for (auto Data : OriginData)
	{
		if (CurRankGrade == Data->RankGrade && CardStat.SuitGrade == Data->SuitGrade)
		{
			FindData = *Data;
			break;
		}
	}
	
	CardStat.RankGrade = CurRankGrade;
	CardStat.CardSprite = FindData.CardSprite;

	if (!CardStat.CardSprite.IsValid())
		CardStat.CardSprite.LoadSynchronous();

}

void AMyPlayerState::DeleteCards(TArray<class UHandInCard_Info*>& CardInfos)
{
	for (UHandInCard_Info* Card : CardInfos)
	{
		CurrentAllHands.Remove(Card);  // 원본에서 삭제
	}

	for (UHandInCard_Info* Card : CardInfos)
	{
		Deck_Stat.Remove(Card);  // 원본에서 삭제
	}

	int32 CurDeckNum = CardInDeckNum;
	CurDeckNum -= CardInfos.Num();

	SetCardInDeckNum(CurDeckNum);


	UE_LOG(LogTemp, Warning, TEXT("CurrentAllHands Num :%d "), CurrentAllHands.Num());

}

FString AMyPlayerState::BossImagePath()
{
	FString AssetPath = "";

	switch (CurrentBostType.Value)
	{
	case EBossType::HOOK:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_Hook.M_Hook");
		break;
	case EBossType::OX:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_OX.M_OX");
		break;
	case EBossType::WALL:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_Wall.M_Wall");
		break;
	case EBossType::ARM:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_ARM.M_ARM");
		break;
	case EBossType::PSYCHIC:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_PSYCHIC.M_PSYCHIC");
		break;
	case EBossType::GOAD:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_GOAD.M_GOAD");
		break;
	case EBossType::WATER:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_WATER.M_WATER");
		break;
	case EBossType::EYE:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_EYE.M_EYE");
		break;
	case EBossType::FINAL:
		AssetPath = TEXT("/Game/UI/View/PlayerInfo/M_FINAL.M_FINAL");
		break;
	default:
		break;
	}

	return AssetPath;
}

FString AMyPlayerState::BossTypeToString()
{
	const UEnum* EnumPtr = StaticEnum<EBossType>();
	if (!EnumPtr) return TEXT("Invalid");
	return EnumPtr->GetNameStringByValue((int64)CurrentBostType.Value);
}