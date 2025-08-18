
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Item/BoosterPackData.h"
#include "VM_Store.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextButton);
DECLARE_MULTICAST_DELEGATE(FOnReRollButton);

/**
 *
 */

UCLASS()
class BALRATRO_API UVM_Store : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnNextButton	OnNextButton;
	FOnReRollButton OnReRollButton;

public:
	const int32 GetMainItemNum()  const { return MainItemNum; }
	void SetMainItemNum(int32 InValue) {
		MainItemNum = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MainItemNum);
	}

	const int32 GetBoucherNum() const { return BoucherNum; }
	void SetBoucherNum(int32 InValue) {
		BoucherNum = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BoucherNum);
	}

	const int32 GetReRollCost() const { return ReRollCost; }
	void  SetReRollCost(int32 InValue) {
		ReRollCost = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ReRollCost);
	}

	const TArray<EBoosterPackType> GetBoosterPackIndexs() const { return BoosterPackIndexs; }
	void  SetBoosterPackIndexs(TArray<EBoosterPackType>& InValue) {
		BoosterPackIndexs = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BoosterPackIndexs);
	}

	void	NextButtonClicked() { 
		BoosterPackIndexs.Empty();
		OnNextButton.Broadcast(); }

	void	ReRollButtonClicked() { OnReRollButton.Broadcast(); }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 MainItemNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BoucherNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 ReRollCost = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<EBoosterPackType> BoosterPackIndexs;
};
