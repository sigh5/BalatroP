// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/PlayerInfoView/PlayerInfoWidget.h"

#include "Components/TextBlock.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

UPlayerInfoWidget::UPlayerInfoWidget()
{
	ViewModelClass = UVM_PlayerInfo::StaticClass();
	ViewModelName = TEXT("VM_PlayerInfo");
}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerInfoWidget::VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
}
