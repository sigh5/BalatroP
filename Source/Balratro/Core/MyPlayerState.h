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
	int32 GetCurrentHealth() { return CurrentHealth; };
	int32 GetMaxHealth() { return MaxHealth; };

	void SetCurrentHealth(int32 InHealth)
	{
		CurrentHealth = InHealth;
	}

	void SetMaxHealth(int32 InMaxHealth)
	{
		MaxHealth = InMaxHealth;
	}

private:
	int32 CurrentHealth = 0;
	int32 MaxHealth = 0;

	int32 RoundCnt = 0;
	
	int32 Gold = 0;
	int32 MaxGold = 0;
	
	int32 EntiCnt = 0;
	
	int32 MaxHandCount = 0;
	int32 UseHandCount = 0;

	int32 MaxChuckCount = 0;
	int32 UseChuckCount = 0;


	int32 MaxScore = 0;


	BlindValueType CurBlindType;
};
