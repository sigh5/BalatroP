
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
enum class EJokerSFX : uint8 // 조커 포리콜롬 효과
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
	PAIR_DRAINAGE = 1,  // 플레이한 핸즈가 Pair 시 +4배수 추가  (합배수)					// 0
	LAST_HAND_MUL3 = 2,  // 마지막 핸즈 플레이시 x3  배수추가  (곱배수)					// 0
	SPADE = 3,			// 플레이한 스페이드 핸즈마다 3배수 추가 (합배수)					// 0
	GOLD = 4,			// 리워드 창에서 5골드 추가  
	COPY = 5,			// 오른쪽 조커 능력 배낌
	ACE_PLUS = 6,			// 플레이한 핸즈에 ACE 있을 때 +30 칩 , +4 배수 추가 (합배수)	// 0
	PAIR_MUL_DRANGE = 7, // 곱배수														// 0
	JOKER_GOLD_SUM = 8,	// 조커의 총 판매금액만큼 배수 증가 (합배수)						// 0
	EMBLEM = 9,		// 스페이드 == 클로버 , 다이아몬드 == 하트								// 0
	HEART_MUL = 10,		// 플레이한 하트 50% 확률로 1.5배 곱배수							// 0
	LEGEND = 11,		// 나중에
	EGG = 12,			// 라운드 종료시 조커 판매금액 +3골드 증가
	STRAIGHT = 13,		// 스트레이트 5->4개로 가능
	PAIR_CHIP = 14,		// pair 시 칩 +50												// 0
	POP_CONE = 15,		// x 2.0 곱배수 (플레이시 마다 0.01 배수씩 깎임)					// 0
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
