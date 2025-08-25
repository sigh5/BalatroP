
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "JokerStat.generated.h"

UENUM(BlueprintType)
enum class EJokerGrade : uint8
{
	NONE = -1,
	NORMAL,
	SCREAT,
	REAR,
	LEGEND,
};

UENUM(BlueprintType)
enum class EJokerSFX : uint8 // 조커 포리콜롬 효과
{
	NONE = -1,
	FOIL, // chip +50
	DRAINAGE_ADD , //  DRAINAGE + 10
	DRAINAGE_MUL,  //  DRAINAGE x 1.5
};


UENUM(BlueprintType)
enum class EJokerType : uint8
{
	NONE,
	BASE_JOKER // 4Drainage

};

USTRUCT(BlueprintType)
struct FJokerStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FJokerStat() : Name(""), JokerType(EJokerType::NONE), JokerGrade(EJokerGrade::NONE), JokerSFX(EJokerSFX::NONE), Price(0), UseNum(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJokerType JokerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJokerGrade JokerGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EJokerSFX	JokerSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UseNum;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> CardSprite;
};


UCLASS(BlueprintType)
class UJokerCard_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FJokerStat Info;
};
