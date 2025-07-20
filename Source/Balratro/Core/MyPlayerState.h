// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "MyPlayerState.generated.h"

UENUM()
enum class BlindValueType :uint8 
{
	BLIND_SKIP = 0,
	SMALL_BLIND,
	BIG_BLIND,
	BOSS_BLIND
};


/**
 * 
 */
UCLASS()
class BALRATRO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
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
	FORCEINLINE void  SetUseChuckCount(int32 InValue) { UseChuckCount = InValue; }

	FORCEINLINE int32 GetCurrentScore() { return CurrentScore; }
	FORCEINLINE void  SetCurrentScore(int32 InValue) { CurrentScore = InValue; }

	FORCEINLINE int32 GetCardInHand() { return CardInHand; }
	FORCEINLINE void  SetCardInHand(int32 InValue) { CardInHand = InValue; }

	FORCEINLINE int32 GetCardInDeck() { return CardInDeck; }
	FORCEINLINE void  SetCardInDeck(int32 InValue) { CardInDeck = InValue; }

	FORCEINLINE int32 GetMaxScore() { return MaxScore; }
	FORCEINLINE void  SetMaxScore(int32 InValue) { MaxScore = InValue; }

	FORCEINLINE const TArray<UHandRanking_Info*>& GetHandRankingNum() const {return MyHandRankingNum;}
	FORCEINLINE void ResetMyHandRankingNum(TArray<UHandRanking_Info*>& InHandRanking) {MyHandRankingNum = InHandRanking;}

	FORCEINLINE TArray<FDeckCardStat*>& GetDeckCardStatTable() { return MyDeckCardStat; }
	FORCEINLINE void ResetDeckCardStatTable(TArray<FDeckCardStat*>& InHandRanking) { MyDeckCardStat = InHandRanking; }
		
private:
	int32 CurrentHealth = 0;
	int32 MaxHealth = 0;

	int32 RoundCount = 0;
	
	int32 Gold = 0;
	int32 MaxGold = 0;
	
	int32 EntiCount = 0;
	
	int32 MaxHandCount = 0;
	int32 UseHandCount = 0;

	int32 MaxChuckCount = 0;
	int32 UseChuckCount = 0;

	int32 CardInHand = 8; // 초기 손에든 패 8장
	int32 CardInDeck = 0; // 초기 Deck 52장


	int32 CurrentScore = 0;
	int32 MaxScore = 0;

	

	BlindValueType CurBlindType;

	TArray<class UHandRanking_Info*> MyHandRankingNum;  // 플레이시 초기화 필수 및 플레이시 내 핸드랭킹

	TArray<FDeckCardStat*> MyDeckCardStat;
};
