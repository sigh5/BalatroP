
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BlindStat.generated.h"

USTRUCT(BlueprintType)
struct FBlindStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBlindStat() : SMallGrade(0), SMallReward(0), BigGrade(0), BigReward(0), BossGrade(0), BossReward(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SMallGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SMallReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BigGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BigReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BossGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32  BossReward;

	bool operator == (const FBlindStat& other) const
	{
		return	SMallGrade == other.SMallGrade &&
				SMallReward == other.SMallReward &&
				BigGrade == other.BigGrade &&
				BigReward == other.BigReward &&
				BossGrade == other.BossGrade &&
				BossReward == other.BossReward;
	}

	bool operator != (const FBlindStat& Other) const
	{
		return !(*this == Other);
	}

};

