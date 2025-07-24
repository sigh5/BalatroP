// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UVM_PlayerInfo : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	const int32 GetRoundCnt() const
	{
		return RoundCnt;
	}

	void SetRoundCnt(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(RoundCnt, InValue);
	}

	//////// Gold/////////////////

	const int32 GetGold() const
	{
		return Gold;
	}

	void SetGold(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Gold, InValue);
	}

	/// /////////////////////////////////////////////////
	const int32 GetEntiCnt() const
	{
		return EntiCnt;
	}
	
	void SetEntiCnt(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(EntiCnt, InValue);
	}


	/// /////////////////////////////////////////////////
	const int32 GetHandCount() const
	{
		return HandCount;
	}

	void SetHandCount(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(HandCount, InValue);
	}

	/// /////////////////////////////////////////////////
	const int32 GetChuckCount() const
	{
		return ChuckCount;
	}

	void SetChuckCount(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(ChuckCount, InValue);
	}

	/// /////////////////////////////////////////////////
	const int32 GetScroe() const
	{
		return Scroe;
	}

	void SetScroe(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Scroe, InValue);
	}

	const FName GetHandName() const
	{
		return HandName;
	}

	void SetHandName(const FName& InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(HandName, InValue);
	}


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RoundCnt = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Gold = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 EntiCnt = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 HandCount = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 ChuckCount = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Scroe = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FName HandName;


	

};
