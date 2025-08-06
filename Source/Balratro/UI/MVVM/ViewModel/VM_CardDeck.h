// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/DeckCardStat.h"
#include "Core\MyPlayerState.h"
#include "VM_CardDeck.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSortTypeChange, const EHandInCardSortType&);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseChuckButton,int32 , TArray<FDeckCardStat>& /*ChuckCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseHandPlayButton, int32, TArray<FDeckCardStat>& /*PlayCardNum*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHandRankName, int32, TArray<FDeckCardStat>& /*PlayCardNum*/);

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
	FOnHandRankName OnHandRankName;
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
		UE_MVVM_SET_PROPERTY_VALUE(CurrentAllHands, InValue);
	}

	void	SetSuitSort() {OnSortTypeChange.Broadcast(EHandInCardSortType::SORT_SUIT);}
	void	SetRankSort() { OnSortTypeChange.Broadcast(EHandInCardSortType::SORT_RANK);}

	void    UseChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) { OnUseChuck.Broadcast(CardNum,_DeckCardStat); }
	void    UseHandPlay(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) { OnUseHandPlay.Broadcast(CardNum, _DeckCardStat); }
	void    BrodCastrHandRankName(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) { OnHandRankName.Broadcast(CardNum, _DeckCardStat); }


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


	const TArray<FDeckCardStat>& GetCurCardsData()  const
	{
		return CurCardsData;
	}

	void	SetCurCardsData(TArray<FDeckCardStat>& _InValue)
	{ 
		UE_MVVM_SET_PROPERTY_VALUE(CurCardsData, _InValue);
	}




private:
	//UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	//int32 DeckNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandInCard_Info*> CurrentAllHands;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool IsUpCardExist;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<FDeckCardStat> CurCardsData;

	UPROPERTY()
	bool IsSelectedMax = false;



};
