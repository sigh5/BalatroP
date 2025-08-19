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


void UStoreView::VM_FieldChanged_BoosterPacks(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	auto Datas = VMInst->GetBoosterPackTypes();

	for (int i = 0; i < Datas.Num(); ++i)
	{ 
		UBoosterPackWidget* BoosterPackButton = CreateWidget<UBoosterPackWidget>(this, BoosterPackWidgetSubClass);
		if (!BoosterPackButton) continue;

		BoosterPackButton->SetInfo(Datas[i]);

		UHorizontalBoxSlot* BoxSlot = PackHorizontalBox->AddChildToHorizontalBox(BoosterPackButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		
		BoosterPackWidget.Add(BoosterPackButton);
	}
}