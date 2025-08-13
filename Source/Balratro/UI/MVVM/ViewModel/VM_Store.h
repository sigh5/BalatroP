
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Store.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextButton);

/**
 * 
 */

UCLASS()
class BALRATRO_API UVM_Store : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnNextButton	OnNextButton;

public:
	const int32 GetMainItemNum()  const  { return MainItemNum; }
	void SetMainItemNum(int32 InValue) { UE_MVVM_SET_PROPERTY_VALUE(MainItemNum, InValue); }

	const int32 GetBoucherNum() const { return BoucherNum; }
	void SetBoucherNum(int32 InValue) { UE_MVVM_SET_PROPERTY_VALUE(BoucherNum, InValue); }

	const int32 GetBosterPackNum() const { return BosterPackNum; }
	void SetBosterPackNum(int32 InValue) { UE_MVVM_SET_PROPERTY_VALUE(BosterPackNum, InValue); }

	const int32 GetReRollCost() const { return ReRollCost; }
	void  SetReRollCost(int32 InValue) { UE_MVVM_SET_PROPERTY_VALUE(ReRollCost, InValue); }


	void	NextButtonClicked() { OnNextButton.Broadcast(); }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 MainItemNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BoucherNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BosterPackNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 ReRollCost = 0;
	

};
