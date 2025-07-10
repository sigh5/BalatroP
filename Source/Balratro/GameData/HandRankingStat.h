
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HandRankingStat.generated.h"


USTRUCT(BlueprintType)
struct FHandRankingStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHandRankingStat() : Level(0), Chip(0), Drainage(0), UseNum(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Chip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Drainage; // ¹è¼ö

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UseNum;


	/*FHandRankingStat operator+(const FHandRankingStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FHandRankingStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FHandRankingStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}*/
};
