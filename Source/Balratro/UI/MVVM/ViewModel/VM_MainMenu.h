// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_MainMenu.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWidgetFlag_Info
{
	GENERATED_BODY()

	FName CurWidgetName;

	bool bActive = false;

	bool operator==(const FWidgetFlag_Info& Other) const
	{
		return CurWidgetName == Other.CurWidgetName
			&& bActive == Other.bActive;
	}


	bool operator!=(const FWidgetFlag_Info& Other) const
	{
		return !(*this == Other);
	}
};

UCLASS()
class BALRATRO_API UVM_MainMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void SetCurWidgetName(const FWidgetFlag_Info&  _InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurWidgetName, _InValue);
	}

	const FWidgetFlag_Info GetCurWidgetName() const {return CurWidgetName;}



private:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FWidgetFlag_Info CurWidgetName;

};
