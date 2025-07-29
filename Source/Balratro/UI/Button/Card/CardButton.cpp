// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/Card/CardButton.h"

#include "Components/HorizontalBoxSlot.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

void UCardButton::SetClikcedEvent()
{
	bSelected = false;
	OnClicked.AddDynamic(this, &UCardButton::OnCardButtonClicked);
}

void UCardButton::OnCardButtonClicked()
{
	auto VM = GetVMCardDeck();
	bool CurSelectedMax = VM->GetIsSelectedMax();

	if (CurSelectedMax == true && bSelected == false) // 5장이 꽉찼고 추가적으로 더 선택할 경우 return;
	{
		return;
	}

	bSelected = !bSelected; // Toggle
	
	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	check(HSlot);
	if (bSelected == false )
	{
		FMargin Margin = HSlot->GetPadding(); 
		Margin.Top += 150.f;  // 선택한 카드 내리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
	else if(bSelected == true && CurSelectedMax == false)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 150.f;    // 선택한 카드 올리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
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
