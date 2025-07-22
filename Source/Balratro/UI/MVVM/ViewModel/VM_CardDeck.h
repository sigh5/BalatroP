// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/DeckCardStat.h"
#include "Core\MyPlayerState.h"
#include "VM_CardDeck.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSortTypeChange, const EHandInCardSortType&);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUseChuckButton,int32 , TArray<FDeckCardStat>& /*ChuckCardNum*/);

/**
 ����, ���, ��� ����
 Ĩ, ��� , �۷��� , ��� ,��ƿ ī�尭ȭ (Ÿ��)
 ����, 1.5���, 10 ���  ���� ī�� ��ȭ
 */


UCLASS()
class BALRATRO_API UVM_CardDeck : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnSortTypeChange  OnSortTypeChange;
	FOnUseChuckButton  OnUseChuckButton;
public:
	const int32 GetDeckNum() const
	{
		return DeckNum;
	}

	void SetDeckNum(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(DeckNum, InValue);
	}

	const TArray<UHandInCard_Info*>& GetCurrentHandInCards() const
	{
		return CurrentHandInCards;
	}

	void SetCurrentHandInCards(const TArray<UHandInCard_Info*>& InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentHandInCards , InValue);
	}

	void	SetSuitSort() {OnSortTypeChange.Broadcast(EHandInCardSortType::SORT_SUIT);}
	void	SetRankSort() { OnSortTypeChange.Broadcast(EHandInCardSortType::SORT_RANK);}

	void    UseChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) { OnUseChuckButton.Broadcast(CardNum,_DeckCardStat); }


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 DeckNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandInCard_Info*> CurrentHandInCards;


};
