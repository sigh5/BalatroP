
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
	ADD_TEN_JKER, // 10���
	MULTIPLE_JOKER, // x1.5
	BOSS_GOLD_REWARD, // 25�� �߰�
	VOUCHER_ADD,      // �ٿ�óī�� 1���߰�
	TWO_COMMON_JOKER, // �Ϲ� ��Ŀ 2�� �߰�
	JUGGLE,		// ���� ���� ���� +3
	BOSS_REROAD,  // ���� ��Ƽ�ɷ� ����
	STANDARD_PACK, // ī�� �� (2�� ����)
	ARCANA_PACK, // Ÿ�� �� (2�� ����)
	ORB_PACK,   // õü �� (2�� ����)
	GHOST_PACK
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
	REWARD,
	ITEM_SELECT,
};

UENUM(BlueprintType)
enum class EBossType :uint8
{
	NONE =0,
	HOOK,	 // �÷����� ������ ������ ī�� 2���� �����ϴ�
	OX,		 // ���� ���� �÷����� ����]�� �÷����ϸ� ���� $0 �� �ȴ�.
	WALL,	 // Ư���� ����ε�. �ּ� ������ �Ϲ� ���� ����ε庸�� 2�� ����.
	ARM,	 // �÷����� ������ ������ �������ϴ�
	PSYCHIC, // �ݵ�� ī�� 5���� �÷����ؾ� �մϴ�.
	GOAD,    // ��� �����̵� ī�尡 ������˴ϴ�.
	WATER,   // 0 ���� ������� �����մϴ�.
	EYE,     // �� ���忡���� �ڵ� ������ �� ������ �÷����� �� �ֽ��ϴ�
	FINAL
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

