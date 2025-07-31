// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/BlindStat.h"
#include "VM_BlindSelect.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectBlind, EPlayerStateType /*BlindSelectType*/);

/**
 * 
 */
UCLASS()
class BALRATRO_API UVM_BlindSelect : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnSelectBlind OnSelectBlind;

public:
	const int32 GetSmallGrade() const
	{
		return SmallGrade;
	}

	void SetSmallGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SmallGrade, InValue);
	}
/// //////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetBigGrade() const
	{
		return BigGrade;
	}

	void SetBigGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BigGrade, InValue);
	}
/// //////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetBossGrade() const
	{
		return BossGrade;
	}

	void SetBossGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BossGrade, InValue);
	}
/// //////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetRoundCnt() const
	{
		return RoundCnt;
	}

	void SetRoundCnt(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(RoundCnt, InValue);
	}

	void SetBlindType(EPlayerStateType InType)
	{
		OnSelectBlind.Broadcast(InType);
	}


	const int32 GetReward() const
	{
		return Reward;
	}


	void SetReward(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Reward, InValue);
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 SmallGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BigGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BossGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RoundCnt = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Reward = 0;
	


};
