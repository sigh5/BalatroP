// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UVM_MainMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void SetCurWidgetName(const FName&  _InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurWidgetName, _InValue);
	}

	const FName GetCurWidgetName() const {return CurWidgetName;}

private:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FName CurWidgetName;

};
