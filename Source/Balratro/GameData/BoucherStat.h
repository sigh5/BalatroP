
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaperSprite.h"
#include "BoucherStat.generated.h"

UENUM(BlueprintType)
enum class EBoucherType :uint8
{
	NONE,		   // �ƹ� ȿ������
	BoucherType_HAND_PULS, // ���� ����
	BoucherType_BOSS_REROLL,   // ���� ����
	BoucherType_ENTI_MINUS,   // ��Ƽ -1 ,�÷��� Ƚ�� -1
	BoucherType_INTEREST_50,
	BoucherType_CHUCK, // ������ ����
	BoucherType_HAND,  // �÷��� Ƚ�� ����
	BoucherType_ORB,  // ����ν����ѿ��� ���� ���� �÷����� �ڵ巩ŷ�� ���� ī�� �׻� ����
	BoucherType_INVENTORY , // �Ҹ�ǰ +1
	BoucherType_COST_DOWN , // ���� 25% ����
	BoucherType_INVENTORY_ORB // �Ҹ�ǰ�� Orb ī�� ������ �ش� orb ī�� ����� x 1.5 �߰� // MOST_ORB �ʿ�
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
	EBoucherType PrevType; // INVENTORY_ORB �����ϱ� ���ؼ��� ����Ÿ�� �ʿ� 
						   // ���� Ÿ��

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
