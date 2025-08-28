// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperSprite.h"
#include "HandRankingToolTip_DataAsset.generated.h"

USTRUCT(BlueprintType)
struct FToolTip_HandRanking
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, meta = (FixedArrayLength = 5))
	TArray<TSoftObjectPtr<UPaperSprite>> Cards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Animmation_Name;
};

/**
 * 
 */
UCLASS()
class BALRATRO_API UHandRankingToolTip_DataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("HandRankingToolTip", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HandRanking)
	TArray<FToolTip_HandRanking> HandRankings;

};
