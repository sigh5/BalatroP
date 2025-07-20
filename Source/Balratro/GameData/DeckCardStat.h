#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DeckCardStat.generated.h"

UENUM(BlueprintType)
enum class SealStatType : uint8
{
	NONE =0,
	BLUE_SEAL,
	RED_SEAL,
	GOLD_SEAL
};

UENUM(BlueprintType)
enum class EnforceStatType : uint8
{
	NONE = 0,
	CHIP_PLUS,
	DRAINAGE,
	STEEL,
	GOLD,
	GLASS
};

UENUM(BlueprintType)
enum class GhostCardStatType : uint8
{
	NONE = 0,
	FOIL,
	DRAINAGE_PLUS,
	DRAINAGE_MUL,
};

USTRUCT(BlueprintType)
struct FDeckCardStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDeckCardStat() : BaseChip(0), SealType(SealStatType::NONE) , EnforceType(EnforceStatType::NONE) , GhostCardType(GhostCardStatType::NONE){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseChip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	SealStatType SealType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EnforceStatType EnforceType; // ¹è¼ö

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GhostCardStatType GhostCardType;


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

