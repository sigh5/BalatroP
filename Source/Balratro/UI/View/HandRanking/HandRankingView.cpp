// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRanking/HandRankingView.h"

#include <Components/TextBlock.h>
#include "Components\Button.h"
#include "Components/ListView.h"

#include <Blueprint/WidgetLayoutLibrary.h>
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

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
	VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::VM_HandRankingData,
		FFieldValueChangedDelegate::CreateUObject(this, &UHandRankingView::VM_FieldChanged_Status));

	//VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::HandRankingButtonPos,
	//	FFieldValueChangedDelegate::CreateUObject(this, &UHandRankingView::VM_FieldChanged_WidgetPos));

	VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::HRButtonInfo,
		FFieldValueChangedDelegate::CreateUObject(this, &UHandRankingView::VM_FieldChanged_WidgetPos));
}

void UHandRankingView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UHandRankingView::VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object);
	HandRankingListView->SetListItems(VMInstance->GetVM_HandRankingData());
}

void UHandRankingView::VM_FieldChanged_WidgetPos(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object);

	IWidgetPosMoveInterface* TooltipView = Cast<IWidgetPosMoveInterface>(HandRankingToolTipWidget);
	if (TooltipView)
	{
		FVector2D ButtonScreenPos = VMInstance->GetHRButtonInfo().Pos;
		if (ButtonScreenPos.X == 0 && ButtonScreenPos.Y == 0)
		{
			TooltipView->SetWidgetPos(VMInstance->GetHRButtonInfo()._Name, ButtonScreenPos);
			return;
		}
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
	
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ButtonScreenPos /= Scale;

		FVector2D CanvasScreenPos = MyCanvasPanel->GetCachedGeometry().GetAbsolutePosition();
		CanvasScreenPos /= Scale;
		
		FVector2D RelativePosInCanvas = ButtonScreenPos - CanvasScreenPos;
		
		TooltipView->SetWidgetPos(VMInstance->GetHRButtonInfo()._Name,RelativePosInCanvas);
	}
}