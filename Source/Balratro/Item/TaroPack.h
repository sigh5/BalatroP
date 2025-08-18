// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BoosterPackData.h"
#include "TaroPack.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UTaroPack : public UBoosterPackData
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("TaroPack", GetFName());
	}

private:
	uint8 IsBase : 1;

	UPROPERTY(EditAnywhere, Category = TaroPack)
	TSoftObjectPtr<class UPaperSprite> MegaPackMesh;

	UPROPERTY(EditAnywhere, Category = TaroPack)
	TSoftObjectPtr<class UPaperSprite> BasePackMesh;
};
