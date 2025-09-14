#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "GameData/BlindStat.h"
#include "GameData/JokerStat.h"
#include "GameData/TaroStat.h"
#include "GameData/BoucherStat.h"
#include "GameData/EnumDatas.h"

#include "MyPlayerState.generated.h"


// First  Test
// 디버그일때만 사용 릴리즈 일때 꺼야됌
// 초기 시작  Bline_View_TEST, PlayerInfoView_VIEW_TEST ,JokerSlotView_VIEW_TEST
//#define Bline_View_TEST
//#define Store_View_TEST
//#define PlayerInfoView_VIEW_TEST
//#define JokerSlotView_VIEW_TEST
//#define CadDeckView_View_TEST
//#define HandRankingView_View_TEST


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUseChuck, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerUseHandPlay, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectNextScene, EPlayerStateType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBlindSkipRewardSetting, EBlindSkip_Tag);

DECLARE_MULTICAST_DELEGATE(FOnDeckCardNum);
DECLARE_MULTICAST_DELEGATE(FOnCurrentPlayerHandRanking);

DECLARE_MULTICAST_DELEGATE(FOnSetCurrentScore);
DECLARE_MULTICAST_DELEGATE(FOnSetRoundCount);
DECLARE_MULTICAST_DELEGATE(FOnSetCurrentGold);
DECLARE_MULTICAST_DELEGATE(FOnSetEntiCount);
DECLARE_MULTICAST_DELEGATE(FOnShowUIChip); 
DECLARE_MULTICAST_DELEGATE(FOnShowUIDrainage);
DECLARE_MULTICAST_DELEGATE(OnBossSkillEvent);
DECLARE_MULTICAST_DELEGATE(FEntiBossClear);

/**
 * 
 */
UCLASS()
class BALRATRO_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

	FOnSetRoundCount			OnSetRoundCount;
	FOnSetEntiCount				OnSetEntiCount;
	FOnSetCurrentGold			OnSetCurrentGold;
	FOnPlayerUseHandPlay		OnPlayerUseHandPlay;
	FOnPlayerUseChuck			OnPlayerUseChuck;
	FOnCurrentPlayerHandRanking OnCurrentPlayerHandRanking;
	FOnSelectNextScene			OnSelectNextScene;
	FOnSetCurrentScore			OnSetCurrentScore;
	
	FOnDeckCardNum				OnDeckCardNum;
	FOnShowUIChip				OnShowUIChip;
	FOnShowUIDrainage			OnShowUIDrainage;
	OnBossSkillEvent		OnBossSkill_PreEvent;
	FOnBlindSkipRewardSetting	OnBlindSkipRewardSetting;
	FEntiBossClear				EntiBossClear;
public:
	FString BossImagePath();
	FString BossTypeToString();

public:
	FORCEINLINE int32 GetRoundCount() { return RoundCount; }
	FORCEINLINE void  SetRoundCount(int32 InValue) { RoundCount = InValue; OnSetRoundCount.Broadcast(); }

	FORCEINLINE int32 GetEntiCount() { return EntiCount; }
	FORCEINLINE void  SetEntiCount(int32 InValue) { EntiCount = InValue; OnSetEntiCount.Broadcast(); }

	FORCEINLINE int32 GetGold() { return Gold; }
	FORCEINLINE void  SetGold(int32 InValue) { Gold = InValue; SetMaxGold(Gold); OnSetCurrentGold.Broadcast(); }
	FORCEINLINE void  SetBossSkill_GoldZero(int32 InValue) { Gold = InValue; }

	FORCEINLINE int32 GetMaxGold() { return MaxGold; }
	FORCEINLINE void  SetMaxGold(int32 InValue) { MaxScore = FMath::Max(MaxScore, InValue); }

	FORCEINLINE int32 GetMaxHandCount() { return MaxHandCount; }
	FORCEINLINE void  SetMaxHandCount(int32 InValue) { MaxHandCount = InValue; }

	FORCEINLINE int32 GetUseHandCount() { return UseHandCount; }
	FORCEINLINE void  SetUseHandCount(int32 InValue) { UseHandCount = InValue;  OnPlayerUseHandPlay.Broadcast(InValue); }

	FORCEINLINE int32 GetMaxChuckCount() { return MaxChuckCount; }
	FORCEINLINE void  SetMaxChuckCount(int32 InValue) { MaxChuckCount = InValue; }

	FORCEINLINE int32 GetUseChuckCount() { return UseChuckCount; }
	FORCEINLINE void  SetUseChuckCount(int32 InValue) { UseChuckCount = InValue;  OnPlayerUseChuck.Broadcast(UseChuckCount); }

	FORCEINLINE int32 GetCurrentScore() { return CurrentScore; }
	FORCEINLINE void  SetCurrentScore(int32 InValue) { CurrentScore = InValue; OnSetCurrentScore.Broadcast();}

	FORCEINLINE int32 GetCardInHand() { return CardInHand; }
	FORCEINLINE void  SetCardInHand(int32 InValue) { CardInHand = InValue; }

	FORCEINLINE int32 GetCardInDeckNum() { return CardInDeckNum; }
	FORCEINLINE void  SetCardInDeckNum(int32 InValue) { CardInDeckNum = InValue;  OnDeckCardNum.Broadcast(); }

	FORCEINLINE int32 GetMaxScore() { return MaxScore; }
	FORCEINLINE void  SetMaxScore(int32 InValue) { MaxScore = FMath::Max(MaxScore, InValue); AllPlayCount++; }


	FORCEINLINE TArray<class UHandRanking_Info*>& GetHandRankingInfoModify() { return MyHandRankingInfo; }
	FORCEINLINE const TArray<class UHandRanking_Info*>& GetHandRankingInfo() const {return MyHandRankingInfo;}
	void ResetMyHandRankingInfo(const TMap<const FName, FHandRankingStat*>& InHandRanking);

	FORCEINLINE int32 GetRerollCost() { return RerollCost; }
	FORCEINLINE void  SetRerollCost(int32 InValue) { RerollCost = InValue;   }


	FORCEINLINE TArray<class UHandInCard_Info*>& GetDeckStatTableModify()  { return Deck_Stat; }
	FORCEINLINE const TArray<class UHandInCard_Info*>& GetDeckStatTable() const { return Deck_Stat; }
	void ResetDeckStatTable(const TArray<FDeckCardStat*>& InHandRanking);
	
	FORCEINLINE const TArray<class UHandInCard_Info*>& GetCurrentAllHands() const {return CurrentAllHands;}
	FORCEINLINE void SetCurrentAllHands(TArray<class UHandInCard_Info*>& InValue) { CurrentAllHands = InValue;}

	FORCEINLINE void AddHandInCard(class UHandInCard_Info* Info)
	{
		CurrentAllHands.Add(Info);
	}

	FORCEINLINE const TArray<class UHandInCard_Info*>  GetRestCardInHands() const { return CurRestCardInHands; }
	FORCEINLINE void  SetRestCardInHands(TArray<class UHandInCard_Info*> _InValue) {CurRestCardInHands = _InValue;}
	void		UseBossSkill() { OnBossSkill_PreEvent.Broadcast(); }


	FORCEINLINE const EHandInCardSortType& GetCurSortType() const { return CurSortType; }
	FORCEINLINE void  SetCurSortType(EHandInCardSortType& InValue) { CurSortType = InValue; }

	FORCEINLINE const EPokerHand& GetCurHandCard_Type() const { return CurHandCard_Type; }
	FORCEINLINE void  SetCurHandCard_Type(EPokerHand InValue) { CurHandCard_Type = InValue;  OnCurrentPlayerHandRanking.Broadcast(); }

	FORCEINLINE const TArray<class UHandInCard_Info*>& GetCurCalculatorCardInHands() const { return CurCalculatorCardInHands; }
	void  SetCurCalculatorCardInHands(TArray<class UHandInCard_Info*>& InValue);

	FORCEINLINE const TArray<class UHandInCard_Info*>& GetAllCurSelectCard() const { return CurSelectedAllCard; }
	void  SetAllCurSelectCard(TArray<UHandInCard_Info*>& InValue);

	FORCEINLINE const EPlayerStateType GetPlayerState() const { return CurPlayerState; }
	FORCEINLINE void SetPlayerState(EPlayerStateType InType) { CurPlayerState = InType; OnSelectNextScene.Broadcast(CurPlayerState); }

	FORCEINLINE const TArray<class UJokerCard_Info*>& GetCurrentJokerCards() const { return CurJokerCardInfo; }
	FORCEINLINE void SetCurrentJokerCards(TArray<class UJokerCard_Info*>& InValue) { CurJokerCardInfo = InValue;}

	FORCEINLINE void AddCurrentJokerCard(const FJokerStat& Info)
	{
		auto JokerCard = NewObject<UJokerCard_Info>(this);
		JokerCard->Info = Info;
		CurJokerCardInfo.Add(JokerCard);
	}

	void DeleteCurrentJokerCard(const FJokerStat& Info);


	FORCEINLINE int32 GetCurrentShowChip() { return CurrentShowChip; }
	FORCEINLINE void  SetCurrentShowChip(int32 InValue) { CurrentShowChip = InValue;  OnShowUIChip.Broadcast(); }

	FORCEINLINE int32 GetCurrentShowDrainage() { return CurrentShowDrainage; }
	FORCEINLINE void  SetCurrentShowDrainage(int32 InValue) { CurrentShowDrainage = InValue; OnShowUIDrainage.Broadcast(); }

	FORCEINLINE int32 GetCurrentRoundBlindGrade() { return CurrentRoundBlindGrade; }
	FORCEINLINE void  SetCurrentRoundBlindGrade(int32 InValue) { CurrentRoundBlindGrade = InValue; }

	void ReStart();  // 죽었을때 다시 하는거
	void SetNextRound(); // 시작하는 값들 초기화

	FORCEINLINE const class UBoosterPackData* GetSelectPackType() { return CurSelcetPackType; }
	FORCEINLINE void  SetSelectPackType(class UBoosterPackData* InValue) { CurSelcetPackType = InValue; }

	FORCEINLINE const TArray<class UTaroStat_Info*>& GetTaroStatTable()  const { return CurTaroStatTable; }
	FORCEINLINE void  SetTaroStatTable(class TArray<class UTaroStat_Info*>& InValue) { CurTaroStatTable = InValue; }

	FORCEINLINE int32 GetCurSelectTaroNum() { return CurSelectTaroNum; }
	FORCEINLINE void  SetCurSelectTaroNum(int32 InValue) { CurSelectTaroNum = InValue; }


	FORCEINLINE int32 GetHaveBoosterPackNum() { return HaveBoosterPackNum; }
	FORCEINLINE void  SetHaveBoosterPackNum(int32 InValue) { HaveBoosterPackNum = InValue;  FMath::Max(MaxHaveBoosterPackNum, HaveBoosterPackNum);}

	FORCEINLINE int32 GetMaxHaveBoosterPackNum() { return MaxHaveBoosterPackNum; }

	FORCEINLINE int32 GetHaveUpStoreNum() { return HaveUpStoreNum; }
	FORCEINLINE void  SetHaveUpStoreNum(int32 InValue) { HaveUpStoreNum = InValue; }

	FORCEINLINE bool  GetHandPlayFlag() { return HandPlayFlag; }
	FORCEINLINE void  SetHandPlayFlag(bool InValue) { HandPlayFlag = InValue; }

	FORCEINLINE const TArray<FBoucherInfo>&  GetCurBoucherInfo() const { return CurBoucherInfo; }
	FORCEINLINE void  SetCurBoucherInfo(TArray<FBoucherInfo>& InValue) { CurBoucherInfo = InValue; }
	void		AddBoucherType(FBoucherInfo& _InValue);
	
	FORCEINLINE const TPair<int32, EBossType>& GetCurBossType() const { return CurrentBostType; }
	FORCEINLINE void SetCurBossType(TPair<int32, EBossType> _InValue) { CurrentBostType = _InValue; }

	FORCEINLINE void  SetCurBlindSkipReward(EBlindSkip_Tag _InValue) {OnBlindSkipRewardSetting.Broadcast(_InValue);BlindSkipCount++;}

	FORCEINLINE int32 GetAllPlayCount() { return AllPlayCount; }
	
	FORCEINLINE void SetAllChuckCount() { AllChuckCount++; }
	FORCEINLINE int32 GetAllChuckCount() { return AllChuckCount; }

	FORCEINLINE void  AddPreEventJoker(class UJokerCard_Info* JokerCard) { CurShakingEventJoker.Add(JokerCard); }
	FORCEINLINE void ResetEventJoker() { CurShakingEventJoker.Empty(); }

	FORCEINLINE void SetDrawCardNum(int32 _InValue) { DrawCardNum = _InValue;}
	FORCEINLINE const int32 GetDrawCardNum() const { return DrawCardNum; }

	class UHandRanking_Info* MostUseHandRankingName();

	void		ResetInfos();

	void FindRankUpCard(OUT FDeckCardStat& CardStat);
	void	 DeleteCards(TArray<class UHandInCard_Info*>& CardInfos);

	void	EntiBossClearFlag() {
		SetEntiCount(EntiCount + 1);
		EntiBossClear.Broadcast(); }


	FORCEINLINE const bool GetIsHavePriceDownBoucher() const { return IsHavePriceDownBoucher; }


private:
	int32 DrawCardNum = 8;

	int32 RoundCount;
	
	int32 Gold;
	int32 MaxGold;
	
	int32 EntiCount;
	
	int32 MaxHandCount;
	int32 UseHandCount;

	int32 MaxChuckCount;
	int32 UseChuckCount;

	int32 CardInHand = 8; // 초기 손에든 패 8장
	int32 CardInDeckNum; // 초기 Deck 52장

	int32 CurrentScore;
	int32 MaxScore;

	int32 AllPlayCount = 0;
	int32 AllChuckCount = 0;

	int32	CurrentShowChip;	 // 초기값 : 핸드랭킹에 있는 레벨업에 따른 기본 칩
	int32	CurrentShowDrainage; // 초기값 : 핸드랭킹에 있는 레벨업에 따른 기본 배수

	int32	RerollCost = 3;
	int32	HaveBoosterPackNum = 2;
	int32	MaxHaveBoosterPackNum = 2;

	int32	HaveUpStoreNum = 2;
	int32	CurrentRoundBlindGrade = 0;
	int32	CurSelectTaroNum = 0;

	uint8	HandPlayFlag : 1;
	uint8   IsHavePriceDownBoucher : 1;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	EPlayerStateType		CurPlayerState;
	EHandInCardSortType		CurSortType;
	EPokerHand				CurHandCard_Type;

	TPair<int32, EBossType> CurrentBostType;

	UPROPERTY()
	TObjectPtr<class UBoosterPackData> CurSelcetPackType;

	UPROPERTY()
	TArray<class UHandRanking_Info*> MyHandRankingInfo;  // 내 핸드랭킹을 사용한 정보들

	UPROPERTY()
	TArray<class UHandInCard_Info*> Deck_Stat;				// 현재 내 덱에 있는 카드들

	UPROPERTY()
	TArray<class UHandInCard_Info*> CurrentAllHands;  // 플레이시 들고 있는 카드들 (8장있으면 8장) (Deck_Stat에 있는 클라스 가지고옴)

	UPROPERTY()
	TArray<class UHandInCard_Info*> CurCalculatorCardInHands;  // Play시에 점수 계산할 카드들 (1~5장 사이) 단순 (Delay 시간)계산만 하므로 구조체로 만듦

	UPROPERTY()
	TArray<class UHandInCard_Info*> CurSelectedAllCard;  // 선택한 카드들 (점수계산없음)

	UPROPERTY()
	TArray<class UHandInCard_Info*> CurRestCardInHands;  // 플레이시도 카드 빼고 남아있는 카드들

	UPROPERTY()
	TArray<class UJokerCard_Info*> CurJokerCardInfo;  // 내가 가지고 있는 조커

	UPROPERTY()
	TArray<class UTaroStat_Info*>	CurTaroStatTable;   // 현재 내 타로 카드 리스트

	UPROPERTY()
	int32	BlindSkipCount = 0;

	UPROPERTY()
	TArray<FBoucherInfo> CurBoucherInfo;

	UPROPERTY()
	TArray<class UJokerCard_Info*> CurShakingEventJoker;  // 내가 가지고 있는 조커

};
