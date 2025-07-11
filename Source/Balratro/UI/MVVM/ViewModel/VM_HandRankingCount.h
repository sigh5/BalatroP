// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/HandRankingStat.h"
#include "VM_HandRankingCount.generated.h"


UCLASS(BlueprintType)
class UHandRanking_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FHandRankingStat Info;

	UPROPERTY(BlueprintReadOnly)
	FName	_Name;
};

/**
 * 
 */
UCLASS()
class BALRATRO_API UVM_HandRankingCount : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	const TArray<UHandRanking_Info*>& GetHandRankingNum() const
	{
		return HandRankingNum;
	}

	void SetHandRankingNum(TArray<UHandRanking_Info*>& InHandRanking)
	{
		HandRankingNum = InHandRanking;
	}

	void AddHandRankingNum(const FName& Name, const FHandRankingStat& Info)
	{
		auto HandRanking = NewObject<UHandRanking_Info>(this);
		HandRanking->Info = Info;
		HandRanking->_Name = Name;
		HandRankingNum.Add(HandRanking);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HandRankingNum);
	}

	void SetWidgetPos(FVector2D& Pos)
	{
		UE_MVVM_SET_PROPERTY_VALUE(WidgetPos, Pos);
	}

	const FVector2D& GetWidgetPos() const
	{
		return WidgetPos;
	}


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandRanking_Info*> HandRankingNum;


	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FVector2D WidgetPos;
};
