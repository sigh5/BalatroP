
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HandRankingStat.generated.h"

UENUM(BlueprintType)
enum class EPokerHand : uint8
{
	NONE,
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	TRIPLE,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_CARD,
	STRAIGHT_FLUSH,
	ROYAL_FLUSH,
	FIVE_CARD
};


USTRUCT(BlueprintType)
struct FHandRankingStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHandRankingStat() : Level(0), Chip(0), Drainage(0), UseNum(0) , IncreaseChip(0), IncreaseDrainage(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Chip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Drainage; // ¹è¼ö

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UseNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IncreaseChip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IncreaseDrainage;

};

UCLASS(BlueprintType)
class UHandRanking_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FHandRankingStat Info;

	UPROPERTY(BlueprintReadOnly)
	FName	_Name;
};
