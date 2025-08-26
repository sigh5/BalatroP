// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/DeckCardStat.h"
#include "Core\MyPlayerState.h"
#include "VM_CardDeck.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkipButtonClicked);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSortTypeChange, const EHandInCardSortType&);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseChuckButton,int32 , TArray<UHandInCard_Info*>& /*ChuckCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseHandPlayButton, int32, TArray<UHandInCard_Info*>& /*PlayCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurPlayHands, int32, TArray<UHandInCard_Info*>& /*PlayCardNum*/);



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

public:
	//const int32 GetDeckNum() const
	//{
	//	return DeckNum;
	//}

	//void SetDeckNum(int32 InValue)
	//{
	//	UE_MVVM_SET_PROPERTY_VALUE(DeckNum, InValue);
	//}

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

	void SkipButtonClicked() { OnSkipButtonClicked.Broadcast(); }

private:
	//UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	//int32 DeckNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandInCard_Info*> CurrentAllHands;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool IsUpCardExist;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<class UHandInCard_Info*> CurCardsData;  // 핸드 플레이할 때 들어옴

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool ItemSelectFlag = false;


	UPROPERTY()
	bool IsSelectedMax = false;



};
