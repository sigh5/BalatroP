// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_MainMenu.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMainPlayButtonClicked);

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
	FOnMainPlayButtonClicked	OnMainPlayButtonClicked;

public:
	void SetCurWidgetName(const FWidgetFlag_Info&  _InValue)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurWidgetName, _InValue);
	}

	const FWidgetFlag_Info GetCurWidgetName() const {return CurWidgetName;}


	void SetClearFlag(bool _InValue) { ClearFlag = _InValue; UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ClearFlag); }
	const bool GetClearFlag() const { return ClearFlag; }

	void SetMainLogoFlag(bool _InValue) { MainLogoFlag = _InValue; UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MainLogoFlag); }
	const bool GetMainLogoFlag() const { return MainLogoFlag; }


	void	ToMain_FromBlindSelectView() { OnMainPlayButtonClicked.Broadcast(); }

private:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FWidgetFlag_Info CurWidgetName;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool ClearFlag = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool MainLogoFlag = false;

};
