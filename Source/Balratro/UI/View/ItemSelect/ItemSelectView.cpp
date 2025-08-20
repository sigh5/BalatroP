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

	const auto VMInst = TryGetViewModel<UVM_ItemSelect>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	TaroStatSubClass = LoadClass<UItemCardWidget>(nullptr, TEXT("/Game/UI/View/ItemSelectView/WBP_Taro.WBP_Taro_C"));

	VMInst->AddFieldValueChangedDelegate(UVM_ItemSelect::FFieldNotificationClassDescriptor::ShowTaroInfo,
		FFieldValueChangedDelegate::CreateUObject(this, &UItemSelectView::VM_FieldChanged_TaroList));
}

void UItemSelectView::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UItemSelectView::VM_FieldChanged_TaroList(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_ItemSelect>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	auto Datas = VMInst->GetShowTaroInfo();

	for (int i = 0; i < Datas.Num(); ++i)
	{
		UItemCardWidget* ItemButton = CreateWidget<UItemCardWidget>(this, TaroStatSubClass);
		if (!ItemButton) continue;

		ItemButton->SetInfo(&Datas[i]);

		UHorizontalBoxSlot* BoxSlot = ItemPanel->AddChildToHorizontalBox(ItemButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

		//BoosterPackWidget.Add(BoosterPackButton);
	}

}
