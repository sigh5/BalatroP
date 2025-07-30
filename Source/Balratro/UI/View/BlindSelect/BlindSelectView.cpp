// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/BlindSelect/BlindSelectView.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "GameData/BlindStat.h"

UBlindSelectView::UBlindSelectView()
{
	ViewModelClass = UVM_BlindSelect::StaticClass();
	ViewModelName = TEXT("VM_BlindSelect");
}

void UBlindSelectView::NativeConstruct()
{
	Super::NativeConstruct();

	SmallBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnSmallBlindButtonClicked);

	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::SmallGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_SmallBlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BigGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BigBlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BossGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BossBlindGrade));


}

void UBlindSelectView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBlindSelectView::OnSmallBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	VMInst->SetBlindType(EPlayerStateType::SMALL_BLIND);
}

void UBlindSelectView::VM_FieldChanged_SmallBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	FNumberFormattingOptions NumberFormatOptions;
	int32 Grade = VMInst->GetSmallGrade();
	
	SmallBlindScoreText->SetText(FText::AsNumber(Grade, &NumberFormatOptions));
	if (Grade < 10000)
		SmallBlindScoreText->SetJustification(ETextJustify::Center);
	else
		SmallBlindScoreText->SetJustification(ETextJustify::Left);
}

void UBlindSelectView::VM_FieldChanged_BigBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();
	FNumberFormattingOptions NumberFormatOptions;
	int32 Grade = VMInst->GetBigGrade();

	BigBlindScoreText->SetText(FText::AsNumber(Grade, &NumberFormatOptions));

	if (Grade < 10000)
		SmallBlindScoreText->SetJustification(ETextJustify::Center);
	else
		SmallBlindScoreText->SetJustification(ETextJustify::Left);
}

void UBlindSelectView::VM_FieldChanged_BossBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();
	FNumberFormattingOptions NumberFormatOptions;

	int32 Grade = VMInst->GetBossGrade();
	BossBlindScoreText->SetText(FText::AsNumber(Grade), &NumberFormatOptions));

	if (Grade < 10000)
		SmallBlindScoreText->SetJustification(ETextJustify::Center);
	else
		SmallBlindScoreText->SetJustification(ETextJustify::Left);
}
