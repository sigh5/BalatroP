// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/DeckCardStat.h"
#include "GameData/HandRankingStat.h"
#include "CalculatorScoreInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCalculatorScoreInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALRATRO_API ICalculatorScoreInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 CalCulatorHandRanking(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) = 0;

};
