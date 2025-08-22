#include "UI/View/Store/StoreView.h"

#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"

#include "UI/Button/BoosterPack/BoosterPackWidget.h"

UStoreView::UStoreView()
{
	ViewModelName = TEXT("VM_Store");
	ViewModelClass = UVM_Store::StaticClass();
}

void UStoreView::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_Store>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	BoosterPackWidgetSubClass = LoadClass<UBoosterPackWidget>(nullptr, TEXT("/Game/UI/View/StoreView/WBP_BoosterButton.WBP_BoosterButton_C"));


	VMInst->AddFieldValueChangedDelegate(UVM_Store::FFieldNotificationClassDescriptor::BoosterPackTypes,
		FFieldValueChangedDelegate::CreateUObject(this, &UStoreView::VM_FieldChanged_BoosterPacks));

}

void UStoreView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NextButton->OnClicked.AddDynamic(this, &UStoreView::OnNextButton);
	ReRollButton->OnClicked.AddDynamic(this, &UStoreView::OnReRollButton);
}

void UStoreView::OnNextButton()
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	VMInst->NextButtonClicked();
}

void UStoreView::OnReRollButton()
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	VMInst->ReRollButtonClicked();
}

UBoosterPackWidget* UStoreView::ReUseWidget(int32 DataNum,int32 Index,  UBoosterPackData* Data)
{
	UBoosterPackWidget* BoosterPackButton = nullptr;

	if (DataNum > BoosterPackWidgets.Num())
	{
		BoosterPackButton = CreateWidget<UBoosterPackWidget>(this, BoosterPackWidgetSubClass);
	}
	else
	{
		BoosterPackButton = BoosterPackWidgets[Index];
	}
	
	check(BoosterPackButton);
	BoosterPackButton->SetInfo(Data);
	return BoosterPackButton;
}

void UStoreView::VM_FieldChanged_BoosterPacks(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	auto Datas = VMInst->GetBoosterPackTypes();
	int32 DataNum = Datas.Num();

	PackHorizontalBox->ClearChildren();  // 기존 이미지 제거

	for (int i = 0; i < DataNum; ++i)
	{ 
		UBoosterPackWidget* BoosterPackButton = ReUseWidget(DataNum, i, Datas[i]);

		UHorizontalBoxSlot* BoxSlot = PackHorizontalBox->AddChildToHorizontalBox(BoosterPackButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		
		BoosterPackWidgets.Add(BoosterPackButton);
	}
}