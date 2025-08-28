
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Boucher.generated.h"

/**
 *
 */

UCLASS()
class BALRATRO_API UVM_Boucher : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	const int32 GetMainItemNum()  const { return MainItemNum; }
	void SetMainItemNum(int32 InValue) 
	{
		MainItemNum = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MainItemNum);
	}

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 MainItemNum = 0;

};
