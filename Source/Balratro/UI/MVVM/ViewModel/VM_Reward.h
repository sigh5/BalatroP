#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Reward.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCashButtonClicked);

/**
 * 
 */

UCLASS()
class BALRATRO_API UVM_Reward : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnCashButtonClicked	OnCashButtonClicked;

public:
	void SetBlindReward(int32 _InValue) { 
		BlindReward = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BlindReward); 
	}
	const int32 GetBlindReward() const { return BlindReward; }

	void SetRestHands(int32 _InValue) { 
		RestHands =  _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(RestHands); }
	const int32 GetRestHands() const { return RestHands; }

	void SetRestChuck(int32 _InValue) { 
		RestChuck = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(RestChuck); }
	const int32 GetRestChuck() const { return RestChuck; }

	void SetInterest(int32 _InValue) { 
		Interest = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Interest); }
	const int32 GetInterest() const { return Interest; }

	void SetBlindGrade(int32 _InValue) { 
		BlindGrade = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BlindGrade); }
	const int32 GetBlindGrade() const { return BlindGrade; }

	void SetBlindMaterialPath(FString _InValue) {
		BlindMaterialPath = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BlindMaterialPath); }
	const FString GetBlindMaterialPath() const { return BlindMaterialPath; }

	void SetEarnGold(int32 _InValue) { 
		EarnGold = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(EarnGold); }
	const int32 GetEarnGold() const { return EarnGold; }

	void  SetNextButtonClicked() { 
		OnCashButtonClicked.Broadcast(); 
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindReward = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RestHands = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RestChuck = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Interest = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FString BlindMaterialPath ;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 EarnGold = 0;


};
