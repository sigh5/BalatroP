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
	STONE,
	GOLD,
	CHIP_PLUS,
	DRAINAGE,
	WILD,
	LUCKY,
	GLASS,
	STEEL,
	CHANGE_HEART,
	CHANGE_SPADE,
	CHANGE_CLOVER,
	CHANGE_DIAMOND,
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
	FDeckCardStat() : BaseChip(0), SealType(SealStatType::NONE) , EnforceType(EnforceStatType::NONE) , GhostCardType(GhostCardStatType::NONE), UseNum(0) {}

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RankGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SuitGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UseNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> CardSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> EnforceSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> GhostSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> SealSprite;


	bool operator==(const FDeckCardStat& Other) const
	{
		return Name == Other.Name
			&& BaseChip == Other.BaseChip
			&& SealType == Other.SealType
			&& EnforceType == Other.EnforceType
			&& GhostCardType == Other.GhostCardType
			&& RankGrade == Other.RankGrade
			&& SuitGrade == Other.SuitGrade
			&& CardSprite == Other.CardSprite;
	}

	bool operator!=(const FDeckCardStat& Other) const
	{
		return !(*this == Other);
	}
};

UCLASS(BlueprintType)
class UHandInCard_Info :public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FDeckCardStat Info;
};
