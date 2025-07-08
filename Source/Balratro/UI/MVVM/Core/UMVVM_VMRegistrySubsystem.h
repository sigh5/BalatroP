// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UMVVM_VMRegistrySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UUMVVM_VMRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void RegisterViewModels();
	void AddViewModel(class UMVVMViewModelCollectionObject* VMCollection, const TSubclassOf<class UMVVMViewModelBase>& ViewModelClass, FName ViewModelUniqueIdentifier);

};
