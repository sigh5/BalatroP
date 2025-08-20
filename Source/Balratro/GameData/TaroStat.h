
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TaroStat.generated.h"



USTRUCT(BlueprintType)
struct FTaroStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTaroStat() : index(0), weight(0), useNum(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 useNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxUseCardNum;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> SpriteAsset;
};


UCLASS(BlueprintType)
class UTaroStat_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FTaroStat Info;
};

