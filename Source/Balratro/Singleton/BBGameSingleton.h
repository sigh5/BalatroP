// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "GameData/BlindStat.h"
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

	FORCEINLINE const TMap<const FName, FHandRankingStat*>& GetHandRankingStatTable() const { return HandRankingStatTable; }

	FORCEINLINE const TArray<FDeckCardStat*>& GetDeckStatTable() const { return DeckCardStatTable; }

	FORCEINLINE const TArray<FBlindStat*>& GetBlindStat() const { return BlindStatTable; }

private:
	TMap<const FName, FHandRankingStat*> HandRankingStatTable;

	TArray<FDeckCardStat*> DeckCardStatTable;

	TArray<FBlindStat*> BlindStatTable;

	UPROPERTY()
	int32 HandRankingMaxNum;

	UPROPERTY()
	int32 DeckCardNum;

	UPROPERTY()
	int32 BlindStatNum;



};
