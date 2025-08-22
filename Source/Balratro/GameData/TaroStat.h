
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameData/DeckCardStat.h"
#include "TaroStat.generated.h"



USTRUCT(BlueprintType)
struct FTaroStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTaroStat() : index(0), weight(0), useNum(0), maxUseCardNum(0), SealType(0), EnforceType(0), GhostCardType(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 useNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxUseCardNum; // 효과 적용가능한 최대카드 숫자

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SealType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnforceType; // 강화 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GhostCardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> SpriteAsset;

	bool operator==(const FTaroStat& Other) const
	{
		return index == Other.index
			&& weight == Other.weight
			&& useNum == Other.useNum
			&& maxUseCardNum == Other.maxUseCardNum
			&& SealType == Other.SealType
			&& EnforceType == Other.EnforceType
			&& GhostCardType == Other.GhostCardType
			&& SpriteAsset == Other.SpriteAsset;
	}

	bool operator!=(const FTaroStat& Other) const
	{
		return !(*this == Other);
	}

};


UCLASS(BlueprintType)
class UTaroStat_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FTaroStat Info;
};

