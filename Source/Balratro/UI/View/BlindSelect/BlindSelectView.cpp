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

	//IsToolTipView = true;
}

void UBlindSelectView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::SmallBlind_SkipTag,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_SmallBlindSkipImage));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BigBlind_SkipTag,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_BigBlindSkipImage));

	VMInst->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::ResetBlindView,
		FFieldValueChangedDelegate::CreateUObject(this, &UBlindSelectView::VM_FieldChanged_ResetBlindView));

	SmallBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnSmallBlindButtonClicked);
	BigBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBigBlindButtonClicked);
	BosslBlindButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBossBlindButtonClicked);
	SmallSkipButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnSmallBlindSkip_ButtonClicked);
	BiglSkipButton->OnClicked.AddDynamic(this, &UBlindSelectView::OnBigBlindSkip_ButtonClicked);

	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BosslBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	FLinearColor BlackColor = FLinearColor::Black;
	ButtonColor[0] = ButtonColor[1] = FSlateColor(BlackColor);;

	FButtonStyle ButtonStyle = SmallBlindButton->WidgetStyle;
	ButtonColor[2] = ButtonStyle.Normal.TintColor;

	ButtonStyle = BigBlindButton->WidgetStyle;
	ButtonColor[3] = ButtonStyle.Normal.TintColor;

	ButtonStyle = BiglSkipButton->WidgetStyle;
	ButtonColor[4] = ButtonStyle.Normal.TintColor;

}

void UBlindSelectView::OnSmallBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	SmallButtonText->SetText(FText::FromString(TEXT("Victory")));

	SetButtonColorAndOpacity(SmallBlindButton, ButtonColor[0], 0.3f);
	SmallSkipButton->SetRenderOpacity(0.3f);

	SmallBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	SmallSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);

	BigBlindButton->SetVisibility(ESlateVisibility::Visible);
	BiglSkipButton->SetVisibility(ESlateVisibility::Visible);
	
	VMInst->SetBlindType(EPlayerStateType::SMALL_BLIND);
}

void UBlindSelectView::OnBigBlindButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	BigButtonText->SetText(FText::FromString(TEXT("Victory")));

	SetButtonColorAndOpacity(BigBlindButton, ButtonColor[1], 0.3f);
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

	SmallButtonText->SetText(FText::FromString(TEXT("Select")));
	SmallBlindButton->SetVisibility(ESlateVisibility::Visible);
	SmallSkipButton->SetVisibility(ESlateVisibility::Visible);

	BigButtonText->SetText(FText::FromString(TEXT("Select")));
	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);

	SetButtonColorAndOpacity(SmallBlindButton, ButtonColor[2], 1.f);
	SetButtonColorAndOpacity(BigBlindButton, ButtonColor[3], 1.f);

	BiglSkipButton->SetRenderOpacity(1.f);
	SmallSkipButton->SetRenderOpacity(1.f);


	VMInst->SetBlindType(EPlayerStateType::BOSS_BLIND);
}

void UBlindSelectView::OnSmallBlindSkip_ButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	SetButtonColorAndOpacity(SmallBlindButton, ButtonColor[0], 0.3f);
	SmallSkipButton->SetRenderOpacity(0.3f);

	SmallButtonText->SetText(FText::FromString(TEXT("Skip")));
	SmallBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	SmallSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);

	

	BigBlindButton->SetVisibility(ESlateVisibility::Visible);
	BiglSkipButton->SetVisibility(ESlateVisibility::Visible);

	VMInst->SetBlindType(EPlayerStateType::SMALL_BLIND_SKIP);
}

void UBlindSelectView::OnBigBlindSkip_ButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>();

	SetButtonColorAndOpacity(BigBlindButton, ButtonColor[1], 0.3f);
	BiglSkipButton->SetRenderOpacity(0.3f);

	BigButtonText->SetText(FText::FromString(TEXT("Skip")));
	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	BosslBlindButton->SetVisibility(ESlateVisibility::Visible);

	VMInst->SetBlindType(EPlayerStateType::BIG_BLIND_SKIP);
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

	FString AssetPath = VMInst->GetBossBlindImage_AssetPath();

	FStringAssetReference MatRef = AssetPath;
	UMaterialInterface* LoadedMat = Cast<UMaterialInterface>(MatRef.TryLoad());
	if (LoadedMat)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(LoadedMat);
		BossBlindImage->SetBrush(Brush);
	}
}

void UBlindSelectView::VM_FieldChanged_SmallBlindSkipImage(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>(); check(VMInst);

	int32 TagNum = static_cast<int32>(VMInst->GetSmallBlind_SkipTag());

	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/tags/Tag%d.Tag%d"), TagNum, TagNum);
	TSoftObjectPtr<UPaperSprite> Sprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));

	if (!Sprite.IsValid())
	{
		Sprite.LoadSynchronous();
	}

	FSlateBrush Brush;

	Brush.SetResourceObject(Sprite.Get());  // UTexture2D or UObject
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.SetImageSize(FVector2D(64.f, 64.f));

	FButtonStyle ButtonStyle = SmallSkipButton->WidgetStyle;
	ButtonStyle.Normal = Brush;   // 기본 상태
	ButtonStyle.Hovered = Brush;   // 마우스 오버 상태
	ButtonStyle.Pressed = Brush;   // 눌렸을 때 상태

	SmallSkipButton->SetStyle(ButtonStyle);
}

void UBlindSelectView::VM_FieldChanged_BigBlindSkipImage(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>(); check(VMInst);

	int32 TagNum = static_cast<int32>(VMInst->GetBigBlind_SkipTag());


	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/tags/Tag%d.Tag%d"), TagNum, TagNum);
	TSoftObjectPtr<UPaperSprite> Sprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));

	if (!Sprite.IsValid())
	{
		Sprite.LoadSynchronous();
	}

	FSlateBrush Brush;

	Brush.SetResourceObject(Sprite.Get());  // UTexture2D or UObject
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.SetImageSize(FVector2D(64.f, 64.f));
	
	FButtonStyle ButtonStyle = BiglSkipButton->WidgetStyle;
	ButtonStyle.Normal = Brush;   // 기본 상태
	ButtonStyle.Hovered = Brush;   // 마우스 오버 상태
	ButtonStyle.Pressed = Brush;   // 눌렸을 때 상태
	
	BiglSkipButton->SetStyle(ButtonStyle);
}

void UBlindSelectView::VM_FieldChanged_ResetBlindView(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_BlindSelect>(); check(VMInst);

	if (VMInst->GetResetBlindView() == false)
		return;

	SmallButtonText->SetText(FText::FromString(TEXT("Select")));
	SmallBlindButton->SetVisibility(ESlateVisibility::Visible);
	SmallSkipButton->SetVisibility(ESlateVisibility::Visible);

	BigButtonText->SetText(FText::FromString(TEXT("Select")));
	BigBlindButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	BiglSkipButton->SetVisibility(ESlateVisibility::HitTestInvisible);

	SetButtonColorAndOpacity(SmallBlindButton, ButtonColor[2], 1.f);
	SetButtonColorAndOpacity(BigBlindButton, ButtonColor[3], 1.f);

	BiglSkipButton->SetRenderOpacity(1.f);
	SmallSkipButton->SetRenderOpacity(1.f);
}

FString UBlindSelectView::BossTypeToString(EBossType _InType)
{
	const UEnum* EnumPtr = StaticEnum<EBossType>();
	if (!EnumPtr) return TEXT("Invalid");
	return EnumPtr->GetNameStringByValue((int64)_InType);
}

void UBlindSelectView::SetButtonColorAndOpacity(UButton* Button, FSlateColor _InColor, float _Ratio)
{
	FButtonStyle ButtonStyle = Button->WidgetStyle;
	
	ButtonStyle.Normal.TintColor = _InColor;
	ButtonStyle.Hovered.TintColor = _InColor;
	ButtonStyle.Pressed.TintColor = _InColor;

	Button->SetStyle(ButtonStyle);
	Button->SetRenderOpacity(_Ratio);
}