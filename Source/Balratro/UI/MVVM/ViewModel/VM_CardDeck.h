// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/DeckCardStat.h"
#include "GameData/BlindStat.h"
#include "Core\MyPlayerState.h"
#include "VM_CardDeck.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkipButtonClicked);

DECLARE_MULTICAST_DELEGATE(FOnBossSkillOtherInfoChange);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSortTypeChange, const EHandInCardSortType&);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseChuckButton,int32 , TArray<UHandInCard_Info*>& /*ChuckCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseHandPlayButton, int32, TArray<UHandInCard_Info*>& /*PlayCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurPlayHands, int32, TArray<UHandInCard_Info*>& /*PlayCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSwapCards, UHandInCard_Info*, UHandInCard_Info* /* Swap Destination, Swap Source */ );

/**
 레드, 골드, 블루 봉인
 칩, 배수 , 글래스 , 골드 ,스틸 카드강화 (타로)
 포일, 1.5배수, 10 배수  유령 카드 강화
 */


UCLASS()
class BALRATRO_API UVM_CardDeck : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnSortTypeChange  OnSortTypeChange;
	FOnUseChuckButton  OnUseChuck;
	FOnUseHandPlayButton OnUseHandPlay;
	FOnCurPlayHands		OnCurPlayHands;
	FOnSkipButtonClicked OnSkipButtonClicked;
	FOnSwapCards		OnSwapCards;

	FOnBossSkillOtherInfoChange		OnBossSkillOtherInfoChange;

public:
	const TArray<UHandInCard_Info*>& GetCurrentAllHands() const
	{
		return CurrentAllHands;
	}

	void SetCurrentAllHands(const TArray<UHandInCard_Info*>& InValue)
	{
		CurrentAllHands.Empty();
		CurrentAllHands = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurrentAllHands);
	}

	void	SetSuitSort() {OnSortTypeChange.Broadcast(EHandInCardSortType::SORT_SUIT);}
	void	SetRankSort() { OnSortTypeChange.Broadcast(EHandInCardSortType::SORT_RANK);}

	void    UseChuck(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat) { OnUseChuck.Broadcast(CardNum,_DeckCardStat); }
	void    UseHandPlay(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat) { OnUseHandPlay.Broadcast(CardNum, _DeckCardStat); }
	void    BroadCastCurHands(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat) { OnCurPlayHands.Broadcast(CardNum, _DeckCardStat); }


	const bool GetIsUpCardExist() const
	{
		return IsUpCardExist;
	}

	void SetIsUpCardExist(const bool InValue)
	{
		if (InValue == false)
			IsUpCardExist = false;
		else
			UE_MVVM_SET_PROPERTY_VALUE(IsUpCardExist, InValue);
	}

	void	SetIsSelectedMax(bool _InValue) { IsSelectedMax = _InValue; }

	FORCEINLINE bool GetIsSelectedMax() const { return IsSelectedMax; }


	const TArray<UHandInCard_Info*>& GetCurCardsData()  const
	{
		return CurCardsData;
	}

	void	SetCurCardsData(TArray<UHandInCard_Info*>& _InValue)
	{ 
		UE_MVVM_SET_PROPERTY_VALUE(CurCardsData, _InValue);
	}

	const bool GetItemSelectFlag() const
	{
		return ItemSelectFlag;
	}

	void SetItemSelectFlag(const bool InValue)
	{
		ItemSelectFlag = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ItemSelectFlag);
	}

	const EBossType& GetCurrentBossType() const
	{
		return CurrentBossType;
	}

	void SetCurrentBossType(EBossType InValue)
	{
		CurrentBossType = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurrentBossType);
	}


	void SkipButtonClicked() { OnSkipButtonClicked.Broadcast(); }

	void SwapCardData( UHandInCard_Info* SwapDest, UHandInCard_Info* Source)
	{
		OnSwapCards.Broadcast(SwapDest, Source);
	}

	const TArray<UHandInCard_Info*>& GetRestCardDatas()  const
	{
		return RestCardDatas;
	}

	void	SetRestCardDatas(TArray<UHandInCard_Info*>& _InValue)
	{
		RestCardDatas = _InValue;
	}

	const bool GetRestCardEffectFlag() const { return RestCardEffectFlag; }
	void	SetRestCardEffectFlag(bool _InValue) { RestCardEffectFlag = _InValue; UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(RestCardEffectFlag); }

	const bool GetBossSkillUse() const { return BossSkillUse; }
	void	SetBossSkillUse(bool _InValue) { BossSkillUse = _InValue; 
	if (BossSkillUse == true)
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BossSkillUse); 
	}

	void	OtherView_InfoChange() { OnBossSkillOtherInfoChange.Broadcast(); }

	const bool GetIsHandPlayFlag() const
	{
		return IsHandPlayFlag;
	}

	void SetIsHandPlayFlag(const bool InValue)
	{
		IsHandPlayFlag = InValue;
	}

	const int32 GetUseless_EmblemType() const
	{
		return Useless_EmblemType;
	}

	void SetUseless_EmblemType(const int32 InValue)
	{
		Useless_EmblemType = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Useless_EmblemType);
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandInCard_Info*> CurrentAllHands;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool IsUpCardExist;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<class UHandInCard_Info*> CurCardsData;  // 핸드 플레이할 때 들어옴

	UPROPERTY()
	TArray<class UHandInCard_Info*> RestCardDatas;  // 현재 손패에 남아있는 카드들

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool ItemSelectFlag = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool RestCardEffectFlag = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool BossSkillUse = false;

	UPROPERTY()
	bool IsSelectedMax = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EBossType	CurrentBossType;

	
	UPROPERTY()
	bool IsHandPlayFlag = false; // false -> 카드 이펙트 계산 끄기, true 카드 이펙트 게산

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Useless_EmblemType = 0; // 0 None , 1 Space,  2 Dia , 3 Heart , 4  Clover

};
