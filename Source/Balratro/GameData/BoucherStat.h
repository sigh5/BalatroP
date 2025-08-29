
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperSprite.h"
#include "BoucherStat.generated.h"

UENUM(BlueprintType)
enum class EBoucherType :uint8
{
	NONE,		   // 아무 효과없음
	BoucherType_HAND_PULS, // 손패 증가
	BoucherType_BOSS_REROLL,   // 보스 리롤
	BoucherType_ENTI_MINUS,   // 엔티 -1 ,플레이 횟수 -1
	BoucherType_INTEREST_50,
	BoucherType_CHUCK, // 버리기 증가
	BoucherType_HAND,  // 플레이 횟수 증가
	BoucherType_ORB,  // 오브부스터팩에서 가장 많이 플레이한 핸드랭킹의 오브 카드 항상 뽑음
	BoucherType_INVENTORY , // 소모품 +1
	BoucherType_COST_DOWN , // 상점 25% 할인
	BoucherType_INVENTORY_ORB // 소모품에 Orb 카드 있을때 해당 orb 카드 배수에 x 1.5 추가 // MOST_ORB 필요
};


USTRUCT(BlueprintType)
struct FBoucherInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoucherType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoucherType PrevType; // INVENTORY_ORB 구매하기 위해서는 선행타입 필요 
						   // 선행 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UPaperSprite> CardSprite;

	bool operator == (const FBoucherInfo& Other) const 
	{
		return Type == Other.Type;
	}

	bool operator!=(const FBoucherInfo& Other) const
	{
		return !(*this == Other);
	}
};
//
//
UCLASS(BlueprintType)
class BALRATRO_API UBoucherStat :public UPrimaryDataAsset
{
	GENERATED_BODY()

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("BoucherStat", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoucherStat)
	TArray<FBoucherInfo> BoucherInfos;
};
