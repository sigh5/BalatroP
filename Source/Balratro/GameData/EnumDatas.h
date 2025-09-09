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
enum class EBossType :uint8
{
	NONE = 0,
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