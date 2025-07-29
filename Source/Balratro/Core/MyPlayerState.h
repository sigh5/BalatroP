// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "MyPlayerState.generated.h"

UENUM()
enum class EBlindValueType :uint8 
{
	BLIND_SKIP = 0,
	SMALL_BLIND,
	BIG_BLIND,
	BOSS_BLIND
};

UENUM()
enum class EHandInCardSortType : uint8
{
	SORT_RANK = 0,
	SORT_SUIT,

};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUseChuck, int32);
DECLARE_MULTICAST_DELEGATE(FOnCurrentPlayerHandRanking);

/**
 * 
 */
UCLASS()
class BALRATRO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

	FOnPlayerUseChuck OnPlayerUseChuck;
	FOnCurrentPlayerHandRanking OnCurrentPlayerHandRanking;

public:
	FORCEINLINE const int32 GetCurrentHealth() const { return CurrentHealth; };
	FORCEINLINE void SetCurrentHealth(int32 InHealth) { CurrentHealth = InHealth; }
	
	FORCEINLINE const int32 GetMaxHealth() const { return MaxHealth; };
	FORCEINLINE void SetMaxHealth(int32 InMaxHealth) {MaxHealth = InMaxHealth;}

	FORCEINLINE int32 GetRoundCount() { return RoundCount; }
	FORCEINLINE void  SetRoundCount(int32 InValue) { RoundCount = InValue; }

	FORCEINLINE int32 GetGold() { return Gold; }
	FORCEINLINE void  SetGold(int32 InValue) { Gold = InValue; }

	FORCEINLINE int32 GetEntiCount() { return EntiCount; }
	FORCEINLINE void  SetEntiCount(int32 InValue) { EntiCount = InValue; }

	FORCEINLINE int32 GetMaxHandCount() { return MaxHandCount; }
	FORCEINLINE void  SetMaxHandCount(int32 InValue) { MaxHandCount = InValue; }

	FORCEINLINE int32 GetUseHandCount() { return UseHandCount; }
	FORCEINLINE void  SetUseHandCount(int32 InValue) { UseHandCount = InValue; }

	FORCEINLINE int32 GetMaxChuckCount() { return MaxChuckCount; }
	FORCEINLINE void  SetMaxChuckCount(int32 InValue) { MaxChuckCount = InValue; }

	FORCEINLINE int32 GetUseChuckCount() { return UseChuckCount; }
	FORCEINLINE void  SetUseChuckCount(int32 InValue) { UseChuckCount = InValue;  OnPlayerUseChuck.Broadcast(UseChuckCount); }

	FORCEINLINE int32 GetCurrentScore() { return CurrentScore; }
	FORCEINLINE void  SetCurrentScore(int32 InValue) { CurrentScore = InValue; }

	FORCEINLINE int32 GetCardInHand() { return CardInHand; }
	FORCEINLINE void  SetCardInHand(int32 InValue) { CardInHand = InValue; }

	FORCEINLINE int32 GetCardInDeck() { return CardInDeck; }
	FORCEINLINE void  SetCardInDeck(int32 InValue) { CardInDeck = InValue; }

	FORCEINLINE int32 GetMaxScore() { return MaxScore; }
	FORCEINLINE void  SetMaxScore(int32 InValue) { MaxScore = InValue; }

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

private:
	int32 CurrentHealth = 0;
	int32 MaxHealth = 0;

	int32 RoundCount;
	
	int32 Gold;
	int32 MaxGold;
	
	int32 EntiCount;
	
	int32 MaxHandCount;
	int32 UseHandCount;

	int32 MaxChuckCount;
	int32 UseChuckCount;

	int32 CardInHand = 8; // 초기 손에든 패 8장
	int32 CardInDeck; // 초기 Deck 52장


	int32 CurrentScore = 0;
	int32 MaxScore = 0;

	EBlindValueType			CurBlindType;
	EHandInCardSortType		CurSortType;
	EPokerHand				CurHandCard_Type;


	UPROPERTY()
	TArray<class UHandRanking_Info*> MyHandRankingInfo;  // 플레이시 초기화 필수 및 플레이시 내 핸드랭킹

	TArray<FDeckCardStat> Deck_Stat;

	UPROPERTY()
	TArray<class UHandInCard_Info*> CurrentAllHands;

	UPROPERTY()
	TArray<FDeckCardStat> CurCalculatorCardInHands;
};
