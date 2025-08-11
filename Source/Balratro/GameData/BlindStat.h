
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
	JUGGLE,		// ���� ���� ���� +3
	BOSS_REROAD,
	STANDARD_PACK, // ī�� �� (2�� ����)
	ARCANA_PACK, // Ÿ�� �� (2�� ����)
	ORB_PACK,   // õü �� (2�� ����)
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

