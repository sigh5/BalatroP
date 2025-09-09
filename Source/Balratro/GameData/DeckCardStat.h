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
	FOOL = 0, // 이카드 제외 마지막에 타로 카드 생성
	LUCKY,    // 럭키 카드 최대 2 장생성				// 0
	ORB,      // 행성카드 최대 2장 생성
	DRAINAGE,  // 배수카드 최대 2장 생성				// 0
	TARO2,		// 타로카드 최대 2장생성				
	CHIP_PLUS,  // +30칩 카드 최대 2장생성			// 0
	WILD,      //  1장까지 와일드로					// 0
	STEEL,		// 스틸카드 1장생성					// 0
	GLASS,      // 글래스 카드 1장생성				// 0
	GOLD_20,    // 골드 2배 최대 20원 x2
	JOKER_FOIL, // 조커 포일, 1.5 ,10배수, 꽝 중 1개  => 조커 카드 구현먼저 하고
	RANK_UP,	// 최대 2장 랭크업
	DELETE_CARD , // 카드 2장 파괴
	CARD_CHANGE, // 왼쪽 카드 오른쪽 카드로 바꿈
	JOKER_GOLD , // 현재 조커 카드 가치 GOLD 로		=> 조커 카드 구현먼저 하고
	GOLD_CARD , // 최대 2장 골드 카드로 바꿈			// 0
	STONE_CARD, // 최대 1장 스톤카드로 바꿈			// 0
	CHANGE_DIAMOND, // 다이아몬드 3장 바꿈			// 0
	CHANGE_CLOVER, // 클로버 3장 바꿈					// 0
	CHANGE_HEART, // 하트 3장 바꿈					// 0
	CREATE_Joker,  // 조커 카드생성					=> 조커 카드 구현먼저 하고
	CHANGE_SPADE,  // 스페이드 3장 바꿈				// 0
	CREATE_LEGEND, // 전설 조커 카드 생성				=> 조커 카드 구현먼저 하고
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
