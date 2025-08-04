
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "JokerStat.generated.h"



USTRUCT(BlueprintType)
struct FJokerStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FJokerStat() : Name(""), Stat(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stat;

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

UENUM(BlueprintType)
enum class JokerType : uint8
{
	BASE_JOKER // 4Drainage

};
