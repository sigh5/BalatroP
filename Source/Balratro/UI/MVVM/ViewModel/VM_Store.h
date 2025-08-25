
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Item/BoosterPackData.h"
#include "GameData/JokerStat.h"
#include "VM_Store.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextButton);
DECLARE_MULTICAST_DELEGATE(FOnReRollButton);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuyBoosterPack, UBoosterPackData*);

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
	FOnBuyBoosterPack OnBuyBoosterPack;
public:
	const int32 GetMainItemNum()  const { return MainItemNum; }
	void SetMainItemNum(int32 InValue) 
	{
		MainItemNum = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MainItemNum);
	}

	const int32 GetBoucherNum() const { return BoucherNum; }
	void SetBoucherNum(int32 InValue) 
	{
		BoucherNum = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BoucherNum);
	}

	const int32 GetReRollCost() const { return ReRollCost; }
	void  SetReRollCost(int32 InValue)
	{
		ReRollCost = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ReRollCost);
	}

	const TArray<UBoosterPackData*> GetBoosterPackTypes() const { return BoosterPackTypes; }
	void  SetBoosterPackTypes(TArray<UBoosterPackData*>& InValue)
	{
		BoosterPackTypes = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BoosterPackTypes);
	}

	const TArray<UJokerCard_Info*>& GetStoreJokerData() const
	{
		return StoreJokerData;
	}


	void SetStoreJokerData(const TArray<UJokerCard_Info*>& InValue)
	{
		StoreJokerData = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(StoreJokerData);
	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
	void	NextButtonClicked() 
	{ 
		BoosterPackTypes.Empty();
		OnNextButton.Broadcast();
	}

	void	ReRollButtonClicked() { OnReRollButton.Broadcast(); }


	void	OnClickedBuyBoosterPackButton(UBoosterPackData* InData)
	{
		OnBuyBoosterPack.Broadcast(InData);
	}


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 MainItemNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 BoucherNum = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 ReRollCost = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UBoosterPackData*> BoosterPackTypes;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UJokerCard_Info*> StoreJokerData;

};
