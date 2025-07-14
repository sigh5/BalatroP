// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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

	FORCEINLINE int16 GetRoundCount() { return RoundCount; }
	FORCEINLINE void  SetRoundCount(int32 InValue) { RoundCount = InValue; }

	FORCEINLINE int16 GetGold() { return Gold; }
	FORCEINLINE void  SetGold(int32 InValue) { Gold = InValue; }

	FORCEINLINE int16 GetEntiCount() { return EntiCount; }
	FORCEINLINE void  SetEntiCount(int32 InValue) { EntiCount = InValue; }

	FORCEINLINE int16 GetMaxHandCount() { return MaxHandCount; }
	FORCEINLINE void  SetMaxHandCount(int32 InValue) { MaxHandCount = InValue; }

	FORCEINLINE int16 GetUseHandCount() { return UseHandCount; }
	FORCEINLINE void  SetUseHandCount(int32 InValue) { UseHandCount = InValue; }

	FORCEINLINE int16 GetMaxChuckCount() { return MaxChuckCount; }
	FORCEINLINE void  SetMaxChuckCount(int32 InValue) { MaxChuckCount = InValue; }

	FORCEINLINE int16 GetUseChuckCount() { return UseChuckCount; }
	FORCEINLINE void  SetUseChuckCount(int32 InValue) { UseChuckCount = InValue; }

	FORCEINLINE int16 GetCurrentScore() { return CurrentScore; }
	FORCEINLINE void  SetCurrentScore(int32 InValue) { CurrentScore = InValue; }

	FORCEINLINE int16 GetMaxScore() { return MaxScore; }
	FORCEINLINE void  SetMaxScore(int32 InValue) { MaxScore = InValue; }

private:
	int32 CurrentHealth = 0;
	int32 MaxHealth = 0;

	int16 RoundCount = 0;
	
	int16 Gold = 0;
	int16 MaxGold = 0;
	
	int16 EntiCount = 0;
	
	int16 MaxHandCount = 0;
	int16 UseHandCount = 0;

	int16 MaxChuckCount = 0;
	int16 UseChuckCount = 0;

	int32 CurrentScore = 0;
	int32 MaxScore = 0;


	BlindValueType CurBlindType;
};
