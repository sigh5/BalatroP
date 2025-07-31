// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MVVMGameSubsystem.h"
#include "Types/MVVMViewModelContext.h"
#include "BBUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UBBUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	template <typename VMClass>
	VMClass* TryGetViewModel() const
	{
		const auto Collection = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

		FMVVMViewModelContext Context;
		Context.ContextClass = ViewModelClass;
		Context.ContextName = ViewModelName;
		const auto VMInstance = Collection->FindViewModelInstance(Context);
		if (IsValid(VMInstance))
		{
			return Cast<VMClass>(VMInstance);
		}
		checkf(false, TEXT("Unable to find a ViewModel that matches the given ViewModelName and ViewModelClass!"));
		return nullptr;
	}


	template <typename VMClass>
	VMClass* TryGetViewModel(FName _ViewModelName, TSubclassOf<UMVVMViewModelBase> _ViewModelClass) const
	{
		const auto Collection = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

		FMVVMViewModelContext Context;
		Context.ContextClass = _ViewModelClass;
		Context.ContextName = _ViewModelName;
		const auto VMInstance = Collection->FindViewModelInstance(Context);
		if (IsValid(VMInstance))
		{
			return Cast<VMClass>(VMInstance);
		}
		checkf(false, TEXT("Unable to find a ViewModel that matches the given ViewModelName and ViewModelClass!"));
		return nullptr;
	}

protected:
	UPROPERTY(EditAnywhere)
	FName ViewModelName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMVVMViewModelBase> ViewModelClass;

};
