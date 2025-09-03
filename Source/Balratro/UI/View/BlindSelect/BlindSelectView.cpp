// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/BlindSelect/BlindSelectView.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "PaperSprite.h"

#include "GameData/BlindStat.h"

UBlindSelectView::UBlindSelectView()
{
	ViewModelClass = UVM_BlindSelect::StaticClass();
	ViewModelName = TEXT("VM_BlindSelect");

	IsToolTipView = true;
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

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::HandRankingView_BlindSelectFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_HandRankingActive_BlindView));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BossType,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BossTypeChanged));
}

void UBlindSelectView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SmallBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnSmallBlindButtonClicked);
	BigBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBigBlindButtonClicked);
	BosslBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBossBlindButtonClicked);
	SmallSkipButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnSmallBlindSkip_ButtonClicked);
	BiglSkipButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBigBlindSkip_ButtonClicked);


	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BosslBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UBlindSelectView::OnSmallBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	SmallButtonText->SetText(FText::FromString(TEXT("Victory")));

	FButtonStyle ButtonStyle = SmallBlindButton->WidgetStyle;
	FLinearColor GrayColor = FLinearColor::Black;
	ButtonStyle.Normal.TintColor = FSlateColor(GrayColor);
	ButtonStyle.Hovered.TintColor = FSlateColor(GrayColor);
	ButtonStyle.Pressed.TintColor = FSlateColor(GrayColor);

	SmallBlindButton->SetStyle(ButtonStyle);
	SmallBlindButton->SetRenderOpacity(0.3f);
	SmallSkipButton->SetRenderOpacity(0.3f);

	SmallBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	SmallSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);

	BigBlindButton->SetVisibility(ESlateVisibility::Visible);
	BiglSkipButton->SetVisibility(ESlateVisibility::Visible);
	
	//VMInst->SetBlindType(EPlayerStateType::SMALL_BLIND);
	VMInst->SetBlindType(EPlayerStateType::BOSS_BLIND);
}

void UBlindSelectView::OnBigBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	BigButtonText->SetText(FText::FromString(TEXT("Victory")));

	FButtonStyle ButtonStyle = BigBlindButton->WidgetStyle;
	FLinearColor GrayColor = FLinearColor::Black;
	ButtonStyle.Normal.TintColor = FSlateColor(GrayColor);
	ButtonStyle.Hovered.TintColor = FSlateColor(GrayColor);
	ButtonStyle.Pressed.TintColor = FSlateColor(GrayColor);

	BigBlindButton->SetStyle(ButtonStyle);
	BigBlindButton->SetRenderOpacity(0.3f);
	BiglSkipButton->SetRenderOpacity(0.3f);

	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	BosslBlindButton->SetVisibility(ESlateVisibility::Visible);

	VMInst->SetBlindType(EPlayerStateType::BIG_BLIND);
}

void UBlindSelectView::OnBossBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	BosslBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);

	VMInst->SetBlindType(EPlayerStateType::BOSS_BLIND);
}

void UBlindSelectView::OnSmallBlindSkip_ButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	SmallSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);


}

void UBlindSelectView::OnBigBlindSkip_ButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);


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
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>(); check(VMInst);
	
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

void UBlindSelectView::VM_FieldChanged_HandRankingActive_BlindView(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	if (IsToolTipView == true)
	{
		SmallBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);		
		SmallSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		BosslBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);

		SmallButtonText->SetText(FText::FromString(TEXT("Info")));
		BigButtonText->SetText(FText::FromString(TEXT("Info")));
		BosslButtonText->SetText(FText::FromString(TEXT("Info")));
	}
}

void UBlindSelectView::VM_FieldChanged_BossTypeChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>(); check(VMInst);

	auto CurBossType = VMInst->GetBossType();
	FString Name = BossTypeToString(CurBossType);

	BossNameText->SetText(FText::FromString(Name));

	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Boss/%s.%s"), *Name, *Name);
	TSoftObjectPtr<UPaperSprite> AssetImage = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!AssetImage.IsValid())
	{
		AssetImage.LoadSynchronous();
	}

	FSlateBrush BossImageBrush;
	BossImageBrush.SetResourceObject(AssetImage.Get());
	BossImageBrush.DrawAs = ESlateBrushDrawType::Image;
	//BossImageBrush.SetImageSize(FVector2D(100.f, 150.f));
	BossBlindImage->SetBrush(BossImageBrush);
}

FString UBlindSelectView::BossTypeToString(EBossType _InType)
{
	const UEnum* EnumPtr = StaticEnum<EBossType>();
	if (!EnumPtr) return TEXT("Invalid");
	return EnumPtr->GetNameStringByValue((int64)_InType);
}
