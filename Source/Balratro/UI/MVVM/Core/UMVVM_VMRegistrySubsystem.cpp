// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MVVM/Core/UMVVM_VMRegistrySubsystem.h"
#include "MVVMGameSubsystem.h"
#include "MVVMViewModelBase.h"

#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"
#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

void UUMVVM_VMRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Collection.InitializeDependency(UMVVMGameSubsystem::StaticClass());

	RegisterViewModels();
}

void UUMVVM_VMRegistrySubsystem::RegisterViewModels()
{
	const auto MVVMCollection = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	AddViewModel(MVVMCollection, UVM_MainMenu::StaticClass(), FName("VM_MainMenu"));
	AddViewModel(MVVMCollection, UVM_JockerSlot::StaticClass(), FName("VM_JockerSlot"));
	AddViewModel(MVVMCollection, UVM_HandRankingCount::StaticClass(), FName("VM_HandRankingCount"));
	AddViewModel(MVVMCollection, UVM_PlayerInfo::StaticClass(), FName("VM_PlayerInfo"));
}

void UUMVVM_VMRegistrySubsystem::AddViewModel(UMVVMViewModelCollectionObject* VMCollection, const TSubclassOf<UMVVMViewModelBase>& ViewModelClass,
	FName ViewModelUniqueIdentifier)
{
	FMVVMViewModelContext ViewModelContext;
	ViewModelContext.ContextName = ViewModelUniqueIdentifier;
	ViewModelContext.ContextClass = ViewModelClass;

	const auto VMInstance = NewObject<UMVVMViewModelBase>(this, ViewModelClass);
	VMCollection->AddViewModelInstance(ViewModelContext, VMInstance);
}
