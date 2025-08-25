// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/JokerView/JokerSlotWidget.h"

#include <Components/TextBlock.h>
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

#include "UI/MVVM/ViewModel/VM_JockerSlot.h"
#include "UI/Button/JokerCard/JokerCardWidget.h"

UJokerSlotWidget::UJokerSlotWidget()
{
	ViewModelClass = UVM_JockerSlot::StaticClass();
	ViewModelName = TEXT("VM_JockerSlot");
}

void UJokerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	JokerCardSubClass = LoadClass<UJokerCardWidget>(nullptr, TEXT("/Game/UI/View/Joker/WBP_JokerCard.WBP_JokerCard_C"));

	const auto VMInst = TryGetViewModel<UVM_JockerSlot>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));
	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::JokerDatas,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_AddJokerCard));
	
}

void UJokerSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//AddButton->OnClicked.AddDynamic(this, &UJokerSlotWidget::AddClicked);
	//RemoveButton->OnClicked.AddDynamic(this, &UJokerSlotWidget::RemoveClicked);
}

UJokerCardWidget* UJokerSlotWidget::ReuseCardButtonWidget(int32 AllNum, int32 Index, UJokerCard_Info* Data)
{
	UJokerCardWidget* NewButton = nullptr;
	if (JokerButtons.Num() < AllNum)
	{
		NewButton = CreateWidget<UJokerCardWidget>(this, JokerCardSubClass);
		JokerButtons.Add(NewButton);
	}
	else
	{
		NewButton = JokerButtons[Index];
	}

	check(NewButton);
	NewButton->SetInfo(Data->Info);
	NewButton->SetIsStoreHave(false);

	return NewButton;
}

void UJokerSlotWidget::VM_FieldChanged_AddJokerCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object); check(VMInstance);
	auto& JokerDatas = VMInstance->GetJokerDatas();
	int32 DataNums = JokerDatas.Num();

	for (int i = 0; i < DataNums; ++i)
	{
		UJokerCardWidget* NewButton = ReuseCardButtonWidget(DataNums, i, JokerDatas[i]);
		UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(JokerWrapBox->AddChild(NewButton));
		if (BoxSlot)
		{
			BoxSlot->SetHorizontalAlignment(HAlign_Center);
			BoxSlot->SetVerticalAlignment(VAlign_Center);
			BoxSlot->SetPadding(FMargin(10.0f, 10.0f));
			BoxSlot->SetFillEmptySpace(false);  // true로 하면 빈 공간까지 채워서 늘어남
		}
	}

	JokerWrapBox->SetHorizontalAlignment(HAlign_Center);
}