// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRanking/HRBoucherView.h"

#include <Components/TextBlock.h>
#include "Components\Button.h"
#include "Components/ListView.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

#include <Blueprint/WidgetLayoutLibrary.h>
#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/Button/Boucher/BoucherCardWidget.h"

UHRBoucherView::UHRBoucherView()
{
	ViewModelClass = UVM_HandRankingCount::StaticClass();
	ViewModelName = TEXT("VM_HandRankingCount");
}

void UHRBoucherView::NativeConstruct()
{
	Super::NativeConstruct();

	BoucherCardWidgetSubClass = LoadClass<UBoucherCardWidget>(nullptr, TEXT("/Game/UI/View/StoreView/WBP_BoucherCard.WBP_BoucherCard_C"));

	const auto VMInst = TryGetViewModel<UVM_HandRankingCount>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_HandRankingCount::FFieldNotificationClassDescriptor::CurHaveBouchers,
		FFieldValueChangedDelegate::CreateUObject(this, &UHRBoucherView::VM_FieldChanged_ShowBoucherCards));
}

void UHRBoucherView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UHRBoucherView::VM_FieldChanged_ShowBoucherCards(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_HandRankingCount>(Object); check(VMInstance);

	auto BoucherInfo = VMInstance->GetCurHaveBouchers();

	BoucherWrapBox->ClearChildren();

	int32 DataNum = BoucherInfo.Num();

	for (int i = 0; i < DataNum; ++i)
	{
		UBoucherCardWidget* BoucherCard = ReUseBoucherWidget(DataNum, i, BoucherInfo[i]);

		UWrapBoxSlot* BoxSlot = BoucherWrapBox->AddChildToWrapBox(BoucherCard);
		if (BoxSlot)
		{
			BoxSlot->SetHorizontalAlignment(HAlign_Center);
			BoxSlot->SetVerticalAlignment(VAlign_Center);
			BoxSlot->SetPadding(FMargin(10.0f, 10.0f));
			BoxSlot->SetFillEmptySpace(false);  // true로 하면 빈 공간까지 채워서 늘어남
		}

		BoucherCard->SetVisibility(ESlateVisibility::Visible);
	}
}

UBoucherCardWidget* UHRBoucherView::ReUseBoucherWidget(int32 DataNum, int32 Index, FBoucherInfo& Data)
{
	UBoucherCardWidget* BoucherButton = nullptr;

	if (DataNum > BoucherCardWidgets.Num())
	{
		BoucherButton = CreateWidget<UBoucherCardWidget>(this, BoucherCardWidgetSubClass);
		BoucherCardWidgets.Add(BoucherButton);
	}
	else
	{
		BoucherButton = BoucherCardWidgets[Index];
	}

	check(BoucherButton);
	BoucherButton->SetInfo(Data);
	BoucherButton->SetIsStoreHave(false);

	return BoucherButton;
}
