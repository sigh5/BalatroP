
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BlindStat.generated.h"

//UENUM(BlueprintType)
//enum class EBlindSkip_Tag : uint8
//{
//	SECREAT_JOKER = 0 , 
//	REAR_JOKER,
//	NEGERTIVE_JOKER,
//	FOIL_JOKER,
//	BOSTER_PACK_FREE,
//	ADD_TEN_JKER, // 10배수
//	MULTIPLE_JOKER, // x1.5
//	BOSS_GOLD_REWARD, // 25골 추가
//	VOUCHER_ADD,      // 바우처카드 1개추가
//	TWO_COMMON_JOKER, // 일반 조커 2개 추가
//	JUGGLE,		// 다음 라운드 손패 +3
//	BOSS_REROAD,  // 보스 엔티능력 리롤
//	STANDARD_PACK, // 카드 팩 (2장 선택)
//	ARCANA_PACK, // 타로 팩 (2장 선택)
//	ORB_PACK,   // 천체 팩 (2장 선택)
//	GHOST_PACK
//};




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

