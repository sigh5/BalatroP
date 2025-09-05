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
	void SetBlindType(EPlayerStateType InType)
	{
		OnSelectBlind.Broadcast(InType);
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetSmallGrade() const
	{
		return SmallGrade;
	}

	void SetSmallGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SmallGrade, InValue);
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetBigGrade() const
	{
		return BigGrade;
	}

	void SetBigGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BigGrade, InValue);
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetBossGrade() const
	{
		return BossGrade;
	}

	void SetBossGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BossGrade, InValue);
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetRoundCnt() const
	{
		return RoundCnt;
	}

	void SetRoundCnt(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(RoundCnt, InValue);
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	const int32 GetReward() const
	{
		return Reward;
	}

	void SetReward(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Reward, InValue);
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	const bool GetSelectButtonActive() const
	{
		return SelectButtonActive;
	}

	void SetSelectButtonActive(bool InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SelectButtonActive, InValue);
	}

/////////////////////////////////////////////////////////////////////////////////////////////
	
	const bool GetHandRankingView_BlindSelectFlag() const
	{
		return HandRankingView_BlindSelectFlag;
	}

	void SetHandRankingView_BlindSelectFlag(bool InValue)
	{
		HandRankingView_BlindSelectFlag = InValue;

		if (HandRankingView_BlindSelectFlag)
		{
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HandRankingView_BlindSelectFlag);
		}
	}

	const EBossType GetBossType() const
	{
		return BossType;
	}

	void SetBossType(EBossType InValue)
	{
		BossType = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BossType);
	}


	const EBlindSkip_Tag GetSmallBlind_SkipTag() const
	{
		return SmallBlind_SkipTag;
	}

	void SetSmallBlind_SkipTag(EBlindSkip_Tag InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SmallBlind_SkipTag, InValue);
	}

	const EBlindSkip_Tag GetBigBlind_SkipTag() const
	{
		return BigBlind_SkipTag;
	}

	void SetBigBlind_SkipTag(EBlindSkip_Tag InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BigBlind_SkipTag, InValue);
	}

	const bool GetResetBlindView() const
	{
		return ResetBlindView;
	}

	void SetResetBlindView(bool InValue)
	{
		ResetBlindView = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ResetBlindView);	
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 SmallGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BigGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BossGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EBossType BossType = EBossType::NONE;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EBlindSkip_Tag SmallBlind_SkipTag;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EBlindSkip_Tag BigBlind_SkipTag;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RoundCnt = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Reward = 0;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool  SelectButtonActive = false; // 보스블라인드 끝내면 이거 호출해야됌

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool  HandRankingView_BlindSelectFlag = false; // 보스블라인드 끝내면 이거 호출해야됌

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool  ResetBlindView = false; // 보스블라인드 끝내면 이거 호출해야됌
};
