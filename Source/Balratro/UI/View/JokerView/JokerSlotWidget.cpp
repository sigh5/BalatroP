// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/JokerView/JokerSlotWidget.h"

#include <Components/TextBlock.h>
#include "Components/Button.h"
#include "Components/ListView.h"

#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

UJokerSlotWidget::UJokerSlotWidget()
{
	ViewModelClass = UVM_JockerSlot::StaticClass();
	ViewModelName = TEXT("VM_JockerSlot");
}

void UJokerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_JockerSlot>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));
	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::Status,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_Status));
	
	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::Players, 
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_Players));
}

void UJokerSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AddButton->OnClicked.AddDynamic(this, &UJokerSlotWidget::AddClicked);
	RemoveButton->OnClicked.AddDynamic(this, &UJokerSlotWidget::RemoveClicked);
}

void UJokerSlotWidget::VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object);
	StatusTextBlock->SetText(VMInstance->GetStatus() == EPartyStatus::Ready ? FText::FromString("Ready") : FText::FromString(("Not ready!")));
}

void UJokerSlotWidget::VM_FieldChanged_Players(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object);
	PartyListView->SetListItems(VMInstance->GetPlayers());
}

void UJokerSlotWidget::AddClicked()
{
	const auto VMInstance = TryGetViewModel<UVM_JockerSlot>();
	VMInstance->AddPlayer(FText::Format(FText::FromString("Player{0}"), FText::AsNumber(TotalAddedCount++)));
}

void UJokerSlotWidget::RemoveClicked()
{
	const auto VMInstance = TryGetViewModel<UVM_JockerSlot>();
	VMInstance->RemovePlayer(0);
}
