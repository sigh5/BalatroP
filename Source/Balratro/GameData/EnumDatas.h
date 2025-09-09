#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerStateType :uint8
{
	NONE,
	LOGO,
	BLINDSELECT,
	STORE,
	SMALL_BLIND,
	SMALL_BLIND_SKIP,
	BIG_BLIND,
	BIG_BLIND_SKIP,
	BOSS_BLIND,
	REWARD,
	ITEM_SELECT,
	GAME_OVER,
	RESET_GAME,
};

UENUM()
enum class EHandInCardSortType : uint8
{
	SORT_RANK = 0,
	SORT_SUIT,
};

UENUM(BlueprintType)
enum class EBlindSkip_Tag : uint8
{
	SECREAT_JOKER = 0,
	REAR_JOKER,
	NEGERTIVE_JOKER,
	FOIL_JOKER,
	BOSTER_PACK_FREE,
	ADD_TEN_JKER, // 10배수
	MULTIPLE_JOKER, // x1.5
	BOSS_GOLD_REWARD, // 25골 추가
	VOUCHER_ADD,      // 바우처카드 1개추가
	TWO_COMMON_JOKER, // 일반 조커 2개 추가
	JUGGLE,		// 다음 라운드 손패 +3
	BOSS_REROAD,  // 보스 엔티능력 리롤
	STANDARD_PACK, // 카드 팩 (2장 선택)
	ARCANA_PACK, // 타로 팩 (2장 선택)
	ORB_PACK,   // 천체 팩 (2장 선택)
	GHOST_PACK
};

UENUM(BlueprintType)
enum class EBossType :uint8
{
	NONE = 0,
	HOOK,	 // 플레이할 때마다 무작위 카드 2장을 버립니다
	OX,		 // 가장 많이 플레이한 족보]를 플레이하면 돈이 $0 이 된다.
	WALL,	 // 특대형 블라인드. 최소 득점이 일반 보스 블라인드보다 2배 많다.
	ARM,	 // 플레이한 족보의 레벨이 낮아집니다
	PSYCHIC, // 반드시 카드 5장을 플레이해야 합니다.
	GOAD,    // 모든 스페이드 카드가 디버프됩니다.
	WATER,   // 0 번의 버리기로 시작합니다.
	EYE,     // 이 라운드에서는 핸드 유형을 한 번씩만 플레이할 수 있습니다
	FINAL
};

UENUM(BlueprintType)
enum class EPokerHand : uint8
{
	NONE,
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	TRIPLE,
	STRAIGHT,
	FLUSH,
	STRAIGHT_FLUSH,
	FULL_HOUSE,
	FOUR_CARD,
	ROYAL_FLUSH,
	FIVE_CARD,
	FlUSH_FIVE_CARD
};