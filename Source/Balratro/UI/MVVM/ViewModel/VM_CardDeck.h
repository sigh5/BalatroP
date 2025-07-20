// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/DeckCardStat.h"
#include "VM_CardDeck.generated.h"

/**
 ����, ���, ��� ����
 Ĩ, ��� , �۷��� , ��� ,��ƿ ī�尭ȭ (Ÿ��)
 ����, 1.5���, 10 ���  ���� ī�� ��ȭ
 */

UCLASS(BlueprintType)
class UHandInCard_Info :public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FDeckCardStat Info;
};


UCLASS()
class BALRATRO_API UVM_CardDeck : public UMVVMViewModelBase
{
	GENERATED_BODY()

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

	void SetCurrentHandInCards(TArray<UHandInCard_Info*>& InValue)
	{
		CurrentHandInCards = InValue;
	}

	void AddHandInCard(const FDeckCardStat& Info)
	{
		auto HandCard = NewObject<UHandInCard_Info>(this);
		HandCard->Info = Info;
		CurrentHandInCards.Add(HandCard);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurrentHandInCards);
	}


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 DeckNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandInCard_Info*> CurrentHandInCards;

};
