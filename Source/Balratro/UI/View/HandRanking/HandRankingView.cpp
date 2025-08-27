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

	VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::HRButtonInfo,
		FFieldValueChangedDelegate::CreateUObject(this, &UHandRankingView::VM_FieldChanged_WidgetPos));
}

void UHandRankingView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PokerHandButton->OnClicked.AddDynamic(this, &UHandRankingView::OnClicked_PokerHandButton);
	BlindButton->OnClicked.AddDynamic(this, &UHandRankingView::OnClicked_BlindButton);
	VoucherButton->OnClicked.AddDynamic(this, &UHandRankingView::OnClicked_VoucherButton);
	ExitButton->OnClicked.AddDynamic(this, &UHandRankingView::OnClickedExitButton);
}

void UHandRankingView::VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object);
	HandRankingListView->SetListItems(VMInstance->GetVM_HandRankingData());
}

void UHandRankingView::VM_FieldChanged_WidgetPos(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object); check(VMInstance);

	IWidgetPosMoveInterface* TooltipView = Cast<IWidgetPosMoveInterface>(HandRankingToolTipWidget);
	if (TooltipView)
	{
		FVector2D ButtonScreenPos = VMInstance->GetHRButtonInfo().Pos;
		TooltipView->SetHandRanking_ToolTipWidgetPos(VMInstance->GetHRButtonInfo(), ButtonScreenPos);
	}
}

void UHandRankingView::OnClickedExitButton()
{
	const auto VM = TryGetViewModel<UVM_HandRankingCount>(); check(VM);

	VM->HandRankingViewExitButtonClicked();

}

void UHandRankingView::OnClicked_PokerHandButton()
{
	const auto VMInstance = TryGetViewModel<UVM_HandRankingCount>();
	

	HandRankingListView->SetVisibility(ESlateVisibility::Visible);
}

void UHandRankingView::OnClicked_BlindButton()
{

	HandRankingListView->SetVisibility(ESlateVisibility::Collapsed);
}

void UHandRankingView::OnClicked_VoucherButton()
{
}
