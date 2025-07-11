// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRankingView.h"

#include <Components/TextBlock.h>
#include "Components\Button.h"
#include "Components/ListView.h"

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "Interface/WidgetPosMoveInterface.h"

UHandRankingView::UHandRankingView()
{
	ViewModelClass = UVM_HandRankingCount::StaticClass();
	ViewModelName = TEXT("VM_HandRankingCount");
}

void UHandRankingView::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_HandRankingCount>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));
	VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::HandRankingNum,
		FFieldValueChangedDelegate::CreateUObject(this, &UHandRankingView::VM_FieldChanged_Status));

	VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::WidgetPos,
		FFieldValueChangedDelegate::CreateUObject(this, &UHandRankingView::VM_FieldChanged_WidgetPos));
}

void UHandRankingView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UHandRankingView::VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object);
	HandRankingListView->SetListItems(VMInstance->GetHandRankingNum());
}

void UHandRankingView::VM_FieldChanged_WidgetPos(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object);

	IWidgetPosMoveInterface* TooltipView = Cast<IWidgetPosMoveInterface>(HandRankingToolTipWidget);
	if (TooltipView)
	{
		FVector2D WidgetPos = VMInstance->GetWidgetPos();
		TooltipView->SetWidgetPos(WidgetPos);
	}
}