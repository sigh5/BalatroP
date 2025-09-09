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
enum class ETaroSkillType : uint8
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


UENUM(BlueprintType)
enum class ETaroType : uint8
{
	FOOL = 0, // ��ī�� ���� �������� Ÿ�� ī�� ����
	LUCKY,    // ��Ű ī�� �ִ� 2 �����				// 0
	ORB,      // �༺ī�� �ִ� 2�� ����
	DRAINAGE,  // ���ī�� �ִ� 2�� ����				// 0
	TARO2,		// Ÿ��ī�� �ִ� 2�����				
	CHIP_PLUS,  // +30Ĩ ī�� �ִ� 2�����			// 0
	WILD,      //  1����� ���ϵ��					// 0
	STEEL,		// ��ƿī�� 1�����					// 0
	GLASS,      // �۷��� ī�� 1�����				// 0
	GOLD_20,    // ��� 2�� �ִ� 20�� x2
	JOKER_FOIL, // ��Ŀ ����, 1.5 ,10���, �� �� 1��  => ��Ŀ ī�� �������� �ϰ�
	RANK_UP,	// �ִ� 2�� ��ũ��
	DELETE_CARD , // ī�� 2�� �ı�
	CARD_CHANGE, // ���� ī�� ������ ī��� �ٲ�
	JOKER_GOLD , // ���� ��Ŀ ī�� ��ġ GOLD ��		=> ��Ŀ ī�� �������� �ϰ�
	GOLD_CARD , // �ִ� 2�� ��� ī��� �ٲ�			// 0
	STONE_CARD, // �ִ� 1�� ����ī��� �ٲ�			// 0
	CHANGE_DIAMOND, // ���̾Ƹ�� 3�� �ٲ�			// 0
	CHANGE_CLOVER, // Ŭ�ι� 3�� �ٲ�					// 0
	CHANGE_HEART, // ��Ʈ 3�� �ٲ�					// 0
	CREATE_Joker,  // ��Ŀ ī�����					=> ��Ŀ ī�� �������� �ϰ�
	CHANGE_SPADE,  // �����̵� 3�� �ٲ�				// 0
	CREATE_LEGEND, // ���� ��Ŀ ī�� ����				=> ��Ŀ ī�� �������� �ϰ�
};

USTRUCT(BlueprintType)
struct FDeckCardStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDeckCardStat() : BaseChip(0), SealType(SealStatType::NONE) , EnforceType(ETaroSkillType::NONE) , GhostCardType(GhostCardStatType::NONE), UseNum(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseChip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	SealStatType SealType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETaroSkillType EnforceType; 

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
