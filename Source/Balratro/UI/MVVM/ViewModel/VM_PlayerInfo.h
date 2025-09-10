// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/DeckCardStat.h"
#include "GameData/HandRankingStat.h"
#include "GameData/TaroStat.h"
#include "VM_PlayerInfo.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClickedRunInfoButton);
DECLARE_MULTICAST_DELEGATE(FOnRefreshPlayerInfoViewData);
/**
 * 
 */
UCLASS()
class BALRATRO_API UVM_PlayerInfo : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnClickedRunInfoButton OnClickedRunInfoButton;
	FOnRefreshPlayerInfoViewData	OnRefreshPlayerInfoViewData;

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

	const int32 GetCurChip() const
	{
		return CurChip;
	}

	void SetCurChip(int32 InValue)
	{
		CurChip = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurChip);
	}

	const int32 GetCurDrainage() const
	{
		return CurDrainage;
	}

	void SetCurDrainage(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurDrainage, InValue);
	}	

	const int32 GetDeckNum() const
	{
		return DeckNum;
	}

	void SetDeckNum(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(DeckNum, InValue);
	}


	const FName GetMainOrder() const
	{
		return MainOrder;
	}

	void SetMainOrder(const FName& InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MainOrder, InValue);
	}


	const bool GetBlindInfoActive() const
	{
		return BlindInfoActive;
	}

	void SetBlindInfoActive(const bool& InValue)
	{
		//UE_MVVM_SET_PROPERTY_VALUE(BlindInfoActive, InValue); // 바꼈을떄만 올리고 싶다 -> 
		BlindInfoActive = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BlindInfoActive); // 이거는 flag 쓸떄 사용
	}


	const int32 GetBlindReward() const
	{
		return BlindReward;
	}

	void SetBlindReward(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BlindReward, InValue);
	}

	const int32 GetBlindGrade() const
	{
		return BlindGrade;
	}

	void SetBlindGrade(int32 InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BlindGrade, InValue);
	}

	const FString GetBlindMaterialPath() const
	{
		return BlindMaterialPath;
	}

	void SetBlindMaterialPath(FString InValue)
	{
		BlindMaterialPath = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BlindMaterialPath);
	}

	const FLinearColor GetBlindBorderColor() const
	{
		return BlindBorderColor;
	}

	void SetBlindBorderColor(FLinearColor InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(BlindBorderColor, InValue);
	}

	void ClickedRunInfoButton()
	{
		OnClickedRunInfoButton.Broadcast();
	}

	void	RefreshPlayerInfoViewEvent() { OnRefreshPlayerInfoViewData.Broadcast(); }

	const TArray<FTaroStat>& GetUseBoxData() const
	{
		return UseBoxData;
	}

	void SetUseBoxData(TArray<FTaroStat>& InValue)
	{
		//UE_MVVM_SET_PROPERTY_VALUE(BlindGrade, InValue);
		UseBoxData = InValue;

		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(UseBoxData);
	}

	void AddUseBoxData(FTaroStat& Datas)
	{
		if (UseBoxData.Num() > 2)
			return;

		UseBoxData.Add(Datas);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(UseBoxData);
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
	int32 Scroe = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 CurChip = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 CurDrainage = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 DeckNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FName HandName;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FName MainOrder;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool BlindInfoActive = true;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindReward = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FLinearColor BlindBorderColor = FColor::White;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FString BlindMaterialPath ;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<FTaroStat> UseBoxData;

};
