// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/CardDeck/CardDeckView.h"

#include "Components/TextBlock.h"


#include "UI/MVVM/ViewModel/VM_CardDeck.h"

UCardDeckView::UCardDeckView()
{
	ViewModelClass = UVM_CardDeck::StaticClass();
	ViewModelName = TEXT("VM_CardDeck");
}

void UCardDeckView::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::DeckNum,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_DeckNum));
}

void UCardDeckView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCardDeckView::VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_CardDeck>(Object);

	CurrentDeckNumText->SetText(FText::AsNumber(VMInstance->GetDeckNum()));
}
