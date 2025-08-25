#include "UI/View/Store/StoreView.h"

#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"

#include "Components/PanelSlot.h"

#include "Components/Spacer.h"


#include "UI/Button/BoosterPack/BoosterPackWidget.h"
#include "UI/Button/JokerCard/JokerCardWidget.h"

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
	JokerCardWidgetSubClass = LoadClass<UJokerCardWidget>(nullptr, TEXT("/Game/UI/View/Joker/WBP_JokerCard.WBP_JokerCard_C"));

	VMInst->AddFieldValueChangedDelegate(UVM_Store::FFieldNotificationClassDescriptor::BoosterPackTypes,
		FFieldValueChangedDelegate::CreateUObject(this, &UStoreView::VM_FieldChanged_BoosterPacks));

	VMInst->AddFieldValueChangedDelegate(UVM_Store::FFieldNotificationClassDescriptor::StoreJokerData,
		FFieldValueChangedDelegate::CreateUObject(this, &UStoreView::VM_FieldChanged_UpStoreJoker));

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
		BoosterPackWidgets.Add(BoosterPackButton);
	}
	else
	{
		BoosterPackButton = BoosterPackWidgets[Index];
	}
	
	check(BoosterPackButton);
	BoosterPackButton->SetInfo(Data);
	return BoosterPackButton;
}

UJokerCardWidget* UStoreView::ReUseJokerWidget(int32 DataNum, int32 Index, UJokerCard_Info* Data)
{
	UJokerCardWidget* JokerCardButton = nullptr;

	if (DataNum > JokercardWidgets.Num())
	{
		JokerCardButton = CreateWidget<UJokerCardWidget>(this, JokerCardWidgetSubClass);
		JokercardWidgets.Add(JokerCardButton);
	}
	else
	{
		JokerCardButton = JokercardWidgets[Index];
	}

	check(JokerCardButton);
	JokerCardButton->SetInfo(Data->Info);
	JokerCardButton->SetIsStoreHave(true);

	return JokerCardButton;
}

void UStoreView::VM_FieldChanged_BoosterPacks(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	auto Datas = VMInst->GetBoosterPackTypes();
	int32 DataNum = Datas.Num();

	PackHorizontalBox->ClearChildren();  // 기존 이미지 제거

	// 왼쪽 Spacer
	if (UHorizontalBoxSlot* LeftSlot = PackHorizontalBox->AddChildToHorizontalBox(NewObject<USpacer>(this)))
	{
		LeftSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	for (int i = 0; i < DataNum; ++i)
	{ 
		UBoosterPackWidget* BoosterPackButton = ReUseWidget(DataNum, i, Datas[i]);

		UHorizontalBoxSlot* BoxSlot = PackHorizontalBox->AddChildToHorizontalBox(BoosterPackButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		
		BoxSlot->SetHorizontalAlignment(HAlign_Center);
		BoxSlot->SetVerticalAlignment(VAlign_Center);

		
	}

	if (UHorizontalBoxSlot* RightSlot = PackHorizontalBox->AddChildToHorizontalBox(NewObject<USpacer>(this)))
	{
		RightSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
	

}

void UStoreView::VM_FieldChanged_UpStoreJoker(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Store>(); check(VMInst);

	auto Datas = VMInst->GetStoreJokerData();
	int32 DataNum = Datas.Num();

	UpHorizontalBox->ClearChildren();  // 기존 이미지 제거

	if (UHorizontalBoxSlot* LeftSlot = UpHorizontalBox->AddChildToHorizontalBox(NewObject<USpacer>(this)))
	{
		LeftSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	for (int i = 0; i < DataNum; ++i)
	{
		UJokerCardWidget* JokerButton = ReUseJokerWidget(DataNum, i, Datas[i]);
		
		UHorizontalBoxSlot* BoxSlot = UpHorizontalBox->AddChildToHorizontalBox(JokerButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

		BoxSlot->SetHorizontalAlignment(HAlign_Center);
		BoxSlot->SetVerticalAlignment(VAlign_Center);

		JokerButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (UHorizontalBoxSlot* RightSlot = UpHorizontalBox->AddChildToHorizontalBox(NewObject<USpacer>(this)))
	{
		RightSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}


}
