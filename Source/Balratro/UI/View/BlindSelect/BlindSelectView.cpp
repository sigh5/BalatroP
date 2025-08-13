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

	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::SmallGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_SmallBlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BigGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BigBlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BossGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BossBlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::SelectButtonActive,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BlindVisibleActive));
}

void UBlindSelectView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SmallBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnSmallBlindButtonClicked);
	BigBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBigBlindButtonClicked);
}

void UBlindSelectView::OnSmallBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	VMInst->SetBlindType(EPlayerStateType::SMALL_BLIND);

	SmallBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	SmallSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UBlindSelectView::OnBigBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	VMInst->SetBlindType(EPlayerStateType::BIG_BLIND);

	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UBlindSelectView::OnBossBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	VMInst->SetBlindType(EPlayerStateType::BOSS_BLIND);

	BosslBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
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
		BigBlindScoreText->SetJustification(ETextJustify::Center);
	else
		BigBlindScoreText->SetJustification(ETextJustify::Left);
}

void UBlindSelectView::VM_FieldChanged_BossBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();
	FNumberFormattingOptions NumberFormatOptions;

	int32 Grade = VMInst->GetBossGrade();
	BossBlindScoreText->SetText(FText::AsNumber(Grade, &NumberFormatOptions));

	if (Grade < 10000)
		BossBlindScoreText->SetJustification(ETextJustify::Center);
	else
		BossBlindScoreText->SetJustification(ETextJustify::Left);
}

void UBlindSelectView::VM_FieldChanged_BlindVisibleActive(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();
	
	bool Active = VMInst->GetSelectButtonActive();

	if (Active == true)
	{
		SmallBlindButton->SetVisibility(ESlateVisibility::Visible);
		SmallSkipButton->SetVisibility(ESlateVisibility::Visible);
		BigBlindButton->SetVisibility(ESlateVisibility::Visible);
		BiglSkipButton->SetVisibility(ESlateVisibility::Visible);
		BosslBlindButton->SetVisibility(ESlateVisibility::Visible);
	}
}
