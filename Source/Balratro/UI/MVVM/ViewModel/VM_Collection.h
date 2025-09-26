
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/BoucherStat.h"
#include "GameData/JokerStat.h"

#include "VM_Collection.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnShowCollection);

/**
 *
 */

UCLASS()
class BALRATRO_API UVM_Collection : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnShowCollection OnShowCollection;

public:
	const TMap<EBoucherType, bool> GetBoucherCollection()  const { return BoucherCollection; }
	void SetBoucherCollection(TMap<EBoucherType, bool> InValue)
	{
		BoucherCollection = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(BoucherCollection);
	}

	const TMap<UJokerCard_Info*, bool>& GetJokerCollection() const   { return JokerCollection; }
	void SetJokerCollection(TMap<UJokerCard_Info*, bool> InValue)
	{
		JokerCollection = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(JokerCollection);
	}



	/////////////////////
	void ShowCollection() { OnShowCollection.Broadcast(); }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TMap<UJokerCard_Info*,bool> JokerCollection;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TMap<EBoucherType, bool>		   BoucherCollection;

};
