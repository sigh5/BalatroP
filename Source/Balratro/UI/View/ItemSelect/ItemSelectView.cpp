// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/ItemSelect/ItemSelectView.h"

#include "UI/MVVM/ViewModel/VM_ItemSelect.h"
#include "UI/Button/Item/ItemCardWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"


UItemSelectView::UItemSelectView()
{
	ViewModelClass = UVM_ItemSelect::StaticClass();
	ViewModelName = TEXT("VM_ItemSelect");
}

void UItemSelectView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemSelectView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto VMInst = TryGetViewModel<UVM_ItemSelect>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	TaroStatSubClass = LoadClass<UItemCardWidget>(nullptr, TEXT("/Game/UI/View/ItemSelectView/WBP_Taro.WBP_Taro_C"));

	VMInst->AddFieldValueChangedDelegate(UVM_ItemSelect::FFieldNotificationClassDescriptor::ShowTaroInfo,
		FFieldValueChangedDelegate::CreateUObject(this, &UItemSelectView::VM_FieldChanged_TaroList));
}

void UItemSelectView::VM_FieldChanged_TaroList(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_ItemSelect>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	auto Datas = VMInst->GetShowTaroInfo();

	ItemPanel->ClearChildren();  // 기존 이미지 제거

	int32 DataNum = Datas.Num();

	for (int i = 0; i < DataNum; ++i)
	{
		UItemCardWidget* ItemCardWidget = ReUseBoosterPackWidget(DataNum, i, Datas[i]);
			
		UHorizontalBoxSlot* BoxSlot = ItemPanel->AddChildToHorizontalBox(ItemCardWidget);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

		BoosterPackWidgets.Add(ItemCardWidget);
	}

}

UItemCardWidget* UItemSelectView::ReUseBoosterPackWidget(int32 DataNum, int32 Index, FTaroStat& Data)
{
	UItemCardWidget* NewWidget = nullptr;

	if (DataNum > BoosterPackWidgets.Num())
	{
		NewWidget = CreateWidget<UItemCardWidget>(this, TaroStatSubClass);
	}
	else
	{
		NewWidget = BoosterPackWidgets[Index];
	}

	check(NewWidget);
	NewWidget->SetInfo(Data);
	
	return NewWidget;
}
