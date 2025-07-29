
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BlindStat.generated.h"

USTRUCT(BlueprintType)
struct FBlindStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBlindStat() : SMallGrade(0), BigGrade(0), BossGrade(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SMallGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BigGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BossGrade;
};