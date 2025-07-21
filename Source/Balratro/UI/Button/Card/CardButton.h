// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "GameData/DeckCardStat.h"
#include "CardButton.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UCardButton : public UButton
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetCardIndex(int InValue) { CardIndex = InValue; }
	FORCEINLINE void SetCardInfoData(FDeckCardStat& InValue) { CardInfoData = InValue; }



private:
	int CardIndex = 0;
	FDeckCardStat CardInfoData;
};
