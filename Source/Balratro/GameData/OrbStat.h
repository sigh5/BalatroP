
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameData/EnumDatas.h"
#include "OrbStat.generated.h"



USTRUCT(BlueprintType)
struct FOrbStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FOrbStat() : _Type(EPokerHand::NONE){}

	UPROPERTY(BlueprintReadOnly)
	EPokerHand _Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 useNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> SpriteAsset;

	bool operator==(const FOrbStat& Other) const
	{
		return _Type == Other._Type;
	}

	bool operator!=(const FOrbStat& Other) const
	{
		return !(*this == Other);
	}

};

