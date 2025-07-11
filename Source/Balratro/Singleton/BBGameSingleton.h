// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/HandRankingStat.h"
#include "BBGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UBBGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UBBGameSingleton();

	static UBBGameSingleton& Get();

public:
	FORCEINLINE FHandRankingStat* GetHandRankingStat(const FName& _Name) const 
	{ 
		auto HandRankingStat = *HandRankingStatTable.Find(_Name);

		return HandRankingStat;
	}


	UPROPERTY()
	int32 HandRankingMaxNum;

private:
	TMap<const FName,FHandRankingStat*> HandRankingStatTable;
};
