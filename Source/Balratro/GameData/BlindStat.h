
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BlindStat.generated.h"

UENUM(BlueprintType)
enum class EBlindSkip_Tag : uint8
{
	SECREAT_JOKER = 0 , 
	REAR_JOKER,
	NEGERTIVE_JOKER,
	FOIL_JOKER,
	BOSTER_PACK_FREE,
	ADD_TEN_JOKER,
	MULTIPLE_JOKER,
	BOSS_GOLD_REWARD,
	VOUCHER_ADD,
	TWO_COMMON_JOKER,
	JUGGLE,		// 다음 라운드 손패 +3
	BOSS_REROAD,
	STANDARD_PACK, // 카드 팩 (2장 선택)
	ARCANA_PACK, // 타로 팩 (2장 선택)
	ORB_PACK,   // 천체 팩 (2장 선택)
};

UENUM(BlueprintType)
enum class EPlayerStateType :uint8
{
	NONE,
	STORE,
	SMALL_BLIND,
	SMALL_BLIND_SKIP,
	BIG_BLIND,
	BIG_BLIND_SKIP,
	BOSS_BLIND,
	SHOP
};


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

	
};

