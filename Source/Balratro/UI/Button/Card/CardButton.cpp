// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/Card/CardButton.h"

#include "Components/HorizontalBoxSlot.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

void UCardButton::SetClikcedEvent()
{
	OnClicked.AddDynamic(this, &UCardButton::OnCardButtonClicked);
}

void UCardButton::OnCardButtonClicked()
{
	bSelected = !bSelected; // Toggle

	auto VM = GetVMCardDeck();
	

	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	check(HSlot);

	if (!bSelected)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top += 150.f;   // 위로 올린 효과 (아래쪽 여백 줄이기)
		HSlot->SetPadding(Margin);

		bSelected = false;
		VM->SetIsUpCardExist(true);
		
	}
	else
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 150.f;   // 위로 올린 효과 (아래쪽 여백 줄이기)
		HSlot->SetPadding(Margin);

		bSelected = true;
		VM->SetIsUpCardExist(true);
	}
	
	//VM->SetIsUpCardExist(true);
}

UVM_CardDeck* UCardButton::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}
