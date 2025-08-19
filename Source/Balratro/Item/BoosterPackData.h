// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BoosterPackData.generated.h"


UENUM(BlueprintType)
enum class EBoosterPackType : uint8
{
	NONE = 0,
	TARO_BASE,
	TARO_MEGA,
	CARD_BASE,  // 2,3 
	CARD_MEGA,
	ORB_BASE,  // ÃµÃ¼ ÆÑ
	ORB_MEGA,
	JOKER_BASE,
	JOKER_MEGA,
	GHOST_BASE,
	GHOST_MEGA
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
	FORCEINLINE void SetType(EBoosterPackType _inType) { Type = _inType; }
	FORCEINLINE const EBoosterPackType GetType() const { return  Type; }

	FORCEINLINE void SetIndex(int32 _inType) { Index = _inType; }
	FORCEINLINE const int32 GetIndex() const { return  Index; }



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EBoosterPackType Type;

	int32 Index = 0;

public:
	UPROPERTY(EditAnywhere, Category = TaroPack)
	TSoftObjectPtr<class UPaperSprite> PackMesh;
	
};
