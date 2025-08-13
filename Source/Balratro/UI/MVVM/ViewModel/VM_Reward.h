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
	void SetBlindReward(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(BlindReward, _InValue); }
	const int32 GetBlindReward() const { return BlindReward; }

	void SetRestHands(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(RestHands, _InValue); }
	const int32 GetRestHands() const { return RestHands; }

	void SetRestChuck(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(RestChuck, _InValue); }
	const int32 GetRestChuck() const { return RestChuck; }

	void SetInterest(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(Interest, _InValue); }
	const int32 GetInterest() const { return Interest; }

	void SetBlindGrade(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(BlindGrade, _InValue); }
	const int32 GetBlindGrade() const { return BlindGrade; }

	void SetBlindImageIndex(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(BlindImageIndex, _InValue); }
	const int32 GetBlindImageIndex() const { return BlindImageIndex; }

	void SetEarnGold(int32 _InValue) { UE_MVVM_SET_PROPERTY_VALUE(EarnGold, _InValue); }
	const int32 GetEarnGold() const { return EarnGold; }

	void  SetNextButtonClicked() { 
		OnCashButtonClicked.Broadcast(); 
		BlindReward = 0;
		RestHands = -1;
		RestChuck = 0;
		Interest = 0;
		BlindGrade = 0;
		BlindImageIndex = -1;
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindReward = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RestHands = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 RestChuck = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 Interest = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindGrade = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BlindImageIndex = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 EarnGold = 0;


};
