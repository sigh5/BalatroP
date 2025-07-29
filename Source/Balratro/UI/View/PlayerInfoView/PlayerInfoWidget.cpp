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


	const auto VMInst = TryGetViewModel<UVM_PlayerInfo>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::Scroe,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_Status));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::HandName,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandName));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::ChuckCount,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurPlayerChuckCount));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::HandCount,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandCount));


	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::CurChip,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Chip));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::DeckNum,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_DeckNum));

}

void UPlayerInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerInfoWidget::VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	ScoreText->SetText(FText::AsNumber(VMInstance->GetScroe()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandName(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	UseHandNameText->SetText(FText::FromName(VMInstance->GetHandName()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurPlayerChuckCount(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	ChuckCountText->SetText(FText::AsNumber(VMInstance->GetChuckCount()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandCount(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	HandCountText->SetText(FText::AsNumber(VMInstance->GetHandCount()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Chip(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	ChipText->SetText(FText::AsNumber(VMInstance->GetCurChip()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Level(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);


}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Drainage(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
}

void UPlayerInfoWidget::VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	CurrentDeckNumText->SetText(FText::AsNumber(VMInstance->GetDeckNum()));
}
