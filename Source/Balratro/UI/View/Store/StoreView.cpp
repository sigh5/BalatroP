#include "UI/View/Store/StoreView.h"

#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Components/Button.h"



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

	VMInst->AddFieldValueChangedDelegate(UVM_Store::FFieldNotificationClassDescriptor::BoosterPackIndexs,
		FFieldValueChangedDelegate::CreateUObject(this, &UStoreView::VM_FieldChanged_BoosterPacks));

}

void UStoreView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NextButton->OnClicked.AddDynamic(this, &UStoreView::OnNextButton);
}

void UStoreView::VM_FieldChanged_BoosterPacks(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);



}

void UStoreView::OnNextButton()
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	VMInst->NextButtonClicked();
}
