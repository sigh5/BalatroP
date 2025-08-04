// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "GameData/BlindStat.h"
#include "GameData/JokerStat.h"
#include "MyPlayerState.generated.h"


UENUM()
enum class EHandInCardSortType : uint8
{
	SORT_RANK = 0,
	SORT_SUIT,

};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUseChuck, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUseHandPlay, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardBattleScene, EPlayerStateType);


DECLARE_MULTICAST_DELEGATE(FOnDeckCardNum);
DECLARE_MULTICAST_DELEGATE(FOnCurrentPlayerHandRanking);

DECLARE_MULTICAST_DELEGATE(FOnSetCurrentScore);
DECLARE_MULTICAST_DELEGATE(FOnSetRoundCount);
DECLARE_MULTICAST_DELEGATE(FOnSetCurrentGold);
DECLARE_MULTICAST_DELEGATE(FOnSetEntiCount);

/**
 * 
 */
UCLASS()
class BALRATRO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

	FOnSetRoundCount			OnSetRoundCount;
	FOnSetEntiCount				OnSetEntiCount;
	FOnSetCurrentGold			OnSetCurrentGold;
	FOnPlayerUseHandPlay		OnPlayerUseHandPlay;
	FOnPlayerUseChuck			OnPlayerUseChuck;
	FOnCurrentPlayerHandRanking OnCurrentPlayerHandRanking;
	FOnCardBattleScene			OnCardBattleScene;
	FOnSetCurrentScore			OnSetCurrentScore;
	
	FOnDeckCardNum				OnDeckCardNum;

public:
	FORCEINLINE int32 GetRoundCount() { return RoundCount; }
	FORCEINLINE void  SetRoundCount(int32 InValue) { RoundCount = InValue; OnSetRoundCount.Broadcast(); }

	FORCEINLINE int32 GetEntiCount() { return EntiCount; }
	FORCEINLINE void  SetEntiCount(int32 InValue) { EntiCount = InValue; OnSetEntiCount.Broadcast(); }

	FORCEINLINE int32 GetGold() { return Gold; }
	FORCEINLINE void  SetGold(int32 InValue) { Gold = InValue; SetMaxGold(Gold); OnSetCurrentGold.Broadcast(); }

	FORCEINLINE int32 GetMaxGold() { return MaxGold; }
	FORCEINLINE void  SetMaxGold(int32 InValue) { MaxScore = FMath::Max(MaxScore, InValue); }

	FORCEINLINE int32 GetMaxHandCount() { return MaxHandCount; }
	FORCEINLINE void  SetMaxHandCount(int32 InValue) { MaxHandCount = InValue; }

	FORCEINLINE int32 GetUseHandCount() { return UseHandCount; }
	FORCEINLINE void  SetUseHandCount(int32 InValue) { UseHandCount = InValue;  OnPlayerUseHandPlay.Broadcast(InValue); }

	FORCEINLINE int32 GetMaxChuckCount() { return MaxChuckCount; }
	FORCEINLINE void  SetMaxChuckCount(int32 InValue) { MaxChuckCount = InValue; }

	FORCEINLINE int32 GetUseChuckCount() { return UseChuckCount; }
	FORCEINLINE void  SetUseChuckCount(int32 InValue) { UseChuckCount = InValue;  OnPlayerUseChuck.Broadcast(UseChuckCount); }

	FORCEINLINE int32 GetCurrentScore() { return CurrentScore; }
	FORCEINLINE void  SetCurrentScore(int32 InValue) { CurrentScore = InValue; SetMaxScore(CurrentScore); OnSetCurrentScore.Broadcast(); }

	FORCEINLINE int32 GetCardInHand() { return CardInHand; }
	FORCEINLINE void  SetCardInHand(int32 InValue) { CardInHand = InValue; }

	FORCEINLINE int32 GetCardInDeckNum() { return CardInDeckNum; }
	FORCEINLINE void  SetCardInDeckNum(int32 InValue) { CardInDeckNum = InValue;  OnDeckCardNum.Broadcast(); }

	FORCEINLINE int32 GetMaxScore() { return MaxScore; }
	FORCEINLINE void  SetMaxScore(int32 InValue){ MaxScore = FMath::Max(MaxScore, InValue); }

	FORCEINLINE const TArray<class UHandRanking_Info*>& GetHandRankingInfo() const {return MyHandRankingInfo;}
	void ResetMyHandRankingInfo(const TMap<const FName, FHandRankingStat*>& InHandRanking);


	FORCEINLINE TArray<FDeckCardStat>& GetDeckStatTableModify()  { return Deck_Stat; }
	FORCEINLINE const TArray<FDeckCardStat>& GetDeckStatTable() const { return Deck_Stat; }
	void ResetDeckStatTable(const TArray<FDeckCardStat*>& InHandRanking);
	
	FORCEINLINE TArray<class UHandInCard_Info*>& GetCurrentAllHandsModify()  { return CurrentAllHands; }
	FORCEINLINE const TArray<class UHandInCard_Info*>& GetCurrentAllHands() const {return CurrentAllHands;}
	FORCEINLINE void SetCurrentAllHands(TArray<class UHandInCard_Info*>& InValue) { CurrentAllHands = InValue;}

	void AddHandInCard(const FDeckCardStat& Info)
	{
		auto HandCard = NewObject<UHandInCard_Info>(this);
		HandCard->Info = Info;
		CurrentAllHands.Add(HandCard);
	}

	FORCEINLINE const EHandInCardSortType& GetCurSortType() const { return CurSortType; }
	FORCEINLINE void  SetCurSortType(EHandInCardSortType& InValue) { CurSortType = InValue; }

	FORCEINLINE const EPokerHand& GetCurHandCard_Type() const { return CurHandCard_Type; }
	FORCEINLINE void  SetCurHandCard_Type(EPokerHand InValue) { CurHandCard_Type = InValue;  OnCurrentPlayerHandRanking.Broadcast(); }


	FORCEINLINE const TArray<FDeckCardStat>& GetCurCalculatorCardInHands() const { return CurCalculatorCardInHands; }
	void  SetCurCalculatorCardInHands(TArray<FDeckCardStat>& InValue);

	FORCEINLINE const EPlayerStateType GetPlayerState() const { return CurPlayerState; }
	FORCEINLINE void SetPlayerState(EPlayerStateType InType) { CurPlayerState = InType; OnCardBattleScene.Broadcast(CurPlayerState); }


	FORCEINLINE TArray<class UJokerCard_Info*>& GetCurrentJokerCardsModify() { return CurJokerCardInfo; }
	FORCEINLINE const TArray<class UJokerCard_Info*>& GetCurrentJokerCards() const { return CurJokerCardInfo; }
	FORCEINLINE void SetCurrentJokerCards(TArray<class UJokerCard_Info*>& InValue) { CurJokerCardInfo = InValue; }

	void AddCurrentJokerCard(const FJokerStat& Info)
	{
		auto JokerCard = NewObject<UJokerCard_Info>(this);
		JokerCard->Info = Info;
		CurJokerCardInfo.Add(JokerCard);
	}


	FORCEINLINE int32 GetCurrentShowChip() { return CurrentShowChip; }
	FORCEINLINE void  SetCurrentShowChip(int32 InValue) { CurrentShowChip = InValue; }

	FORCEINLINE int32 GetCurrentShowDrainage() { return CurrentShowDrainage; }
	FORCEINLINE void  SetCurrentShowDrainage(int32 InValue) { CurrentShowDrainage = InValue; }


private:
	int32 RoundCount;
	
	int32 Gold;
	int32 MaxGold;
	
	int32 EntiCount;
	
	int32 MaxHandCount;
	int32 UseHandCount;

	int32 MaxChuckCount;
	int32 UseChuckCount;

	int32 CardInHand = 8; // 초기 손에든 패 8장
	int32 CardInDeckNum; // 초기 Deck 52장


	int32 CurrentScore = 0;
	int32 MaxScore = 0;

	EPlayerStateType		CurPlayerState;
	EHandInCardSortType		CurSortType;
	EPokerHand				CurHandCard_Type;


	UPROPERTY()
	TArray<class UHandRanking_Info*> MyHandRankingInfo;  // 플레이시 초기화 필수 및 플레이시 내 핸드랭킹

	TArray<FDeckCardStat> Deck_Stat;

	UPROPERTY()
	TArray<class UHandInCard_Info*> CurrentAllHands;

	UPROPERTY()
	TArray<FDeckCardStat> CurCalculatorCardInHands;

	UPROPERTY()
	TArray<class UJokerCard_Info*> CurJokerCardInfo;

	int32		CurrentShowChip;
	int32		CurrentShowDrainage;

};
