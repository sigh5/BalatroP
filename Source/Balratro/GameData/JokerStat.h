
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "JokerStat.generated.h"

UENUM(BlueprintType)
enum class EJokerGrade : uint8
{
	NONE = 0,
	NORMAL,
	SCREAT,
	REAR,
	LEGEND,
};

UENUM(BlueprintType)
enum class EJokerSFX : uint8 // ��Ŀ �����ݷ� ȿ��
{
	NONE = 0,
	FOIL, // chip +50
	DRAINAGE_ADD , //  DRAINAGE + 10
	DRAINAGE_MUL,  //  DRAINAGE x 1.5
};


UENUM(BlueprintType)
enum class EJokerType : uint8
{
	BASE_JOKER = 0,    // Last Cal + 4Drainage											// 0
	PAIR_DRAINAGE = 1,  // �÷����� ��� Pair �� +4��� �߰�  (�չ��)					// 0
	LAST_HAND_MUL3 = 2,  // ������ ���� �÷��̽� x3  ����߰�  (�����)					// 0
	SPADE = 3,			// �÷����� �����̵� ����� 3��� �߰� (�չ��)					// 0
	GOLD = 4,			// ������ â���� 5��� �߰�  
	COPY = 5,			// ������ ��Ŀ �ɷ� �賦
	ACE_PLUS = 6,			// �÷����� ��� ACE ���� �� +30 Ĩ , +4 ��� �߰� (�չ��)	// 0
	PAIR_MUL_DRANGE = 7, // �����														// 0
	JOKER_GOLD_SUM = 8,	// ��Ŀ�� �� �Ǹűݾ׸�ŭ ��� ���� (�չ��)						// 0
	EMBLEM = 9,		// �����̵� == Ŭ�ι� , ���̾Ƹ�� == ��Ʈ								// 0
	HEART_MUL = 10,		// �÷����� ��Ʈ 50% Ȯ���� 1.5�� �����							// 0
	LEGEND = 11,		// ���߿�
	EGG = 12,			// ���� ����� ��Ŀ �Ǹűݾ� +3��� ����
	STRAIGHT = 13,		// ��Ʈ����Ʈ 5->4���� ����
	PAIR_CHIP = 14,		// pair �� Ĩ +50												// 0
	POP_CONE = 15,		// x 2.0 ����� (�÷��̽� ���� 0.01 ����� ����)					// 0
	NONE = 99,
};


USTRUCT(BlueprintType)
struct FJokerStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FJokerStat() : Name(""), JokerType(EJokerType::BASE_JOKER), JokerGrade(EJokerGrade::NONE), JokerSFX(EJokerSFX::NONE), Price(0), UseNum(0) {}

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

	bool operator==(const FJokerStat& Other) const
	{
		return Name == Other.Name
			&& JokerType == Other.JokerType
			&& JokerGrade == Other.JokerGrade
			&& JokerSFX == Other.JokerSFX
			&& Price == Other.Price
			&& UseNum == Other.UseNum;
	}

	bool operator!=(const FJokerStat& Other) const
	{
		return !(*this == Other);
	}

};


UCLASS(BlueprintType)
class UJokerCard_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FJokerStat Info;
};
