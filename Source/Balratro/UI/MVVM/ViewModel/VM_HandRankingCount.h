// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/HandRankingStat.h"
#include "VM_HandRankingCount.generated.h"

USTRUCT(BlueprintType)
struct FHRButton_Info 
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadOnly)
	FVector2D Pos;

	UPROPERTY(BlueprintReadOnly)
	FName	_Name;

	FHRButton_Info(): Pos(FVector2D::ZeroVector), _Name(NAME_None){}

	// 비교 연산자 정의
	bool operator==(const FHRButton_Info& Other) const
	{
		return Pos == Other.Pos && _Name == Other._Name;
	}

	bool operator!=(const FHRButton_Info& Other) const
	{
		return !(*this == Other);
	}
};



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

	const FHRButton_Info GetHRButtonInfo() const { return HRButtonInfo; }

	void SetHRButtonInfo(const FHRButton_Info& InArg)
	{
		UE_MVVM_SET_PROPERTY_VALUE(HRButtonInfo, InArg);
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandRanking_Info*> HandRankingNum;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FHRButton_Info HRButtonInfo;


};
