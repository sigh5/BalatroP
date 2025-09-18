#include "UI/View/HandRanking/HandRankingView.h"

#include <Components/TextBlock.h>
#include "Components\Button.h"
#include "Components/ListView.h"

#include <Blueprint/WidgetLayoutLibrary.h>
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

#include "Interface/WidgetPosMoveInterface.h"

#include "UI/View/BlindSelect/BlindSelectView.h"

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

	BlindSelectToolTipWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	
	BlindSelectToolTipWidget->SetVisibility(ESlateVisibility::Collapsed);
	BoucherWidget->SetVisibility(ESlateVisibility::Collapsed);
	HandRankingListView->SetVisibility(ESlateVisibility::Visible);
}

void UHandRankingView::OnClicked_BlindButton()
{
	const auto VM_BlindInfo = TryGetViewModel<UVM_BlindSelect>("VM_BlindSelect", UVM_BlindSelect::StaticClass()); check(VM_BlindInfo);

	BlindSelectToolTipWidget->SetVisibility(ESlateVisibility::Visible);
	HandRankingListView->SetVisibility(ESlateVisibility::Collapsed);
	BoucherWidget->SetVisibility(ESlateVisibility::Collapsed);

	VM_BlindInfo->SetHandRankingView_BlindSelectFlag(true);
}

void UHandRankingView::OnClicked_VoucherButton()
{
	const auto VMInstance = TryGetViewModel<UVM_HandRankingCount>();

	BlindSelectToolTipWidget->SetVisibility(ESlateVisibility::Collapsed);
	HandRankingListView->SetVisibility(ESlateVisibility::Collapsed);
	BoucherWidget->SetVisibility(ESlateVisibility::Visible);

	VMInstance->VoucherButtonShowButton();
}
