// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoosterPackData.generated.h"


UENUM(BlueprintType)
enum class EBoosterPackType : uint8
{
	TARO = 0,
	CARD,
	ORB,  // ÃµÃ¼ ÆÑ
	JOKER,
	GHOST
};


/**
 * 
 */
UCLASS()
class BALRATRO_API UBoosterPackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("UBoosterPackData", GetFName());
	}

public:
	//UBoosterPackData();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EBoosterPackType Type;
};
