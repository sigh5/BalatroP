// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/PlayerInfoView/PlayerInfoWidget.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"


#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

#include "Animation/WidgetAnimation.h"

UPlayerInfoWidget::UPlayerInfoWidget()
{
	ViewModelClass = UVM_PlayerInfo::StaticClass();
	ViewModelName = TEXT("VM_PlayerInfo");
}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FillAnimMap();

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MainOrderText->Slot);
	check(CanvasSlot);
	MainOrderPos = CanvasSlot->GetPosition();

	const auto VMInst = TryGetViewModel<UVM_PlayerInfo>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::Scroe,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_Score));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::HandName,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandName));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::ChuckCount,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurPlayerChuckCount));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::HandCount,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandCount));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::CurChip,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Chip));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::CurDrainage,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Drainage));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::DeckNum,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_DeckNum));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::MainOrder,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_MainOrder));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::BlindInfoActive,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindInfoActive));
	
	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::BlindReward,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindReward));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::Gold,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_Gold));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::EntiCnt,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_EntiCount));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::RoundCnt,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_RoundCount));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::BlindGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::BlindBorderColor,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindNameBorderColor));


	VMInst->AddFieldValueChangedDelegate(UVM_PlayerInfo::FFieldNotificationClassDescriptor::BlindImageIndex,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindPresentImage));

}

void UPlayerInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerInfoWidget::VM_FieldChanged_Score(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	FNumberFormattingOptions NumberFormatOptions;
	int32 Grade = VMInstance->GetScroe();

	ScoreText->SetText(FText::AsNumber(Grade, &NumberFormatOptions));
	if (Grade < 10000)
		ScoreText->SetJustification(ETextJustify::Center);
	else
		ScoreText->SetJustification(ETextJustify::Left);
}

void UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandName(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	UseHandNameText->SetText(FText::FromName(VMInstance->GetHandName()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurPlayerChuckCount(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	ChuckCountText->SetText(FText::AsNumber(VMInstance->GetChuckCount()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurPlayerHandCount(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	HandCountText->SetText(FText::AsNumber(VMInstance->GetHandCount()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Chip(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	int Value = VMInstance->GetCurChip();

	FNumberFormattingOptions NumberFormatOptions;
	ChipText->SetText(FText::AsNumber(Value, &NumberFormatOptions));
	if (Value < 10000)
		ChipText->SetJustification(ETextJustify::Center);
	else
		ChipText->SetJustification(ETextJustify::Left);
}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Drainage(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	int Value = VMInstance->GetCurDrainage();

	FString MyFString = DrainageText->GetText().ToString();
	int PrevDrainage = FCString::Atoi(*MyFString);
	if (Value != PrevDrainage)
	{
		FName CurAnimNaim = TEXT("DrainageChangeEvent");
		UWidgetAnimation* Anim = GetAnimationByName(CurAnimNaim);
		PlayAnimation(Anim);
	}

	FNumberFormattingOptions NumberFormatOptions;
	DrainageText->SetText(FText::AsNumber(Value, &NumberFormatOptions));
	if (Value < 10000)
		DrainageText->SetJustification(ETextJustify::Center);
	else
		DrainageText->SetJustification(ETextJustify::Left);
}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Level(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
}

void UPlayerInfoWidget::VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	CurrentDeckNumText->SetText(FText::AsNumber(VMInstance->GetDeckNum()));
}

void UPlayerInfoWidget::VM_FieldChanged_Gold(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	GoldText->SetText(FText::AsNumber(VMInstance->GetGold()));
}

void UPlayerInfoWidget::VM_FieldChanged_EntiCount(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	FText Str = FText::Format(NSLOCTEXT("MyCount", "EntiCount", "{0}/8"), FText::AsNumber(VMInstance->GetEntiCnt()));
	EntiText->SetText(Str);
}

void UPlayerInfoWidget::VM_FieldChanged_RoundCount(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	RoundText->SetText(FText::AsNumber(VMInstance->GetRoundCnt()));
}

void UPlayerInfoWidget::VM_FieldChanged_MainOrder(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	FName CurMainOrderName = VMInstance->GetMainOrder();
	MainOrderText->SetText(FText::FromName(CurMainOrderName));

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MainOrderText->Slot);
	check(CanvasSlot);
	FSlateFontInfo FontInfo = MainOrderText->Font;

	FVector2D CurrentPosition = CanvasSlot->GetPosition();
	if (VMInstance->GetBlindInfoActive())
	{
		CanvasSlot->SetPosition(FVector2D(MainOrderPos.X , MainOrderPos.Y - 50.f));
		FontInfo.Size = 40;
		MainOrderText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	}
	else
	{
		CanvasSlot->SetPosition(MainOrderPos);
		FontInfo.Size = 35;
		MainOrderText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	}

	MainOrderText->SetFont(FontInfo);
}

void UPlayerInfoWidget::VM_FieldChanged_BlindInfoActive(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	
	bool Flag = VMInstance->GetBlindInfoActive();
	if (Flag)
	{
		CurBlindNameBorder->SetVisibility(ESlateVisibility::Visible);
		CurBlindChipImage->SetVisibility(ESlateVisibility::Visible);
		BlindScoreBorder->SetVisibility(ESlateVisibility::Visible);
		CurBlindGrade->SetVisibility(ESlateVisibility::Visible);
		CurBlindNoneText->SetVisibility(ESlateVisibility::Visible);
		CurBlindPresentImage->SetVisibility(ESlateVisibility::Visible);
		RewardText->SetVisibility(ESlateVisibility::Visible);
		RewardResultText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CurBlindNameBorder->SetVisibility(ESlateVisibility::Collapsed);
		CurBlindChipImage->SetVisibility(ESlateVisibility::Collapsed);
		BlindScoreBorder->SetVisibility(ESlateVisibility::Collapsed);
		CurBlindGrade->SetVisibility(ESlateVisibility::Collapsed);
		CurBlindNoneText->SetVisibility(ESlateVisibility::Collapsed);
		CurBlindPresentImage->SetVisibility(ESlateVisibility::Collapsed);
		RewardText->SetVisibility(ESlateVisibility::Collapsed);
		RewardResultText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerInfoWidget::VM_FieldChanged_BlindReward(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	
	int32 Reward = VMInstance->GetBlindReward();
	
	if (Reward == -1)
	{
		RewardResultText->SetText(FText::FromString(TEXT("-")));
	}
	
	FString RewardStr = "";

	for (int i = 0; i < Reward; ++i)
	{
		RewardStr += "$";
	}
	RewardStr += " + a";

	RewardResultText->SetText(FText::FromString(RewardStr));
}

void UPlayerInfoWidget::VM_FieldChanged_BlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);

	int32 Grade = VMInstance->GetBlindGrade();
	FNumberFormattingOptions NumberFormatOptions;
	CurBlindGrade->SetText(FText::AsNumber(Grade, &NumberFormatOptions));
	if (Grade < 10000)
		CurBlindGrade->SetJustification(ETextJustify::Center);
	else
		CurBlindGrade->SetJustification(ETextJustify::Left);
}

void UPlayerInfoWidget::VM_FieldChanged_BlindNameBorderColor(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	CurBlindNameBorder->SetBrushColor(FLinearColor::White); // 초기화
	FLinearColor LinearColor = VMInstance->GetBlindBorderColor();
	CurBlindNameBorder->SetBrushColor(LinearColor);
}

void UPlayerInfoWidget::VM_FieldChanged_BlindPresentImage(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	int Index = VMInstance->GetBlindImageIndex();

	FString AssetPath = "";
	TSoftObjectPtr<UPaperSprite> MyAsset;
	if (Index == 0)
	{
		AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Blind/BlindChips_Sprite_0.BlindChips_Sprite_0"));
	}
	else if (Index == 1)
	{
		AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Blind/BlindChips_Sprite_21.BlindChips_Sprite_21"));
	}
	else if (Index == 2) /// 보스 블라인드 이미지가 달라서
	{

	}
	
	MyAsset = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (MyAsset.IsValid())
	{
		MyAsset.LoadSynchronous();
	}

	UPaperSprite* Sprite = MyAsset.Get();
	FSlateBrush SpriteBrush;
	SpriteBrush.SetResourceObject(Sprite);
	SpriteBrush.ImageSize = FVector2D(100.f, 150.f);
	SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
	CurBlindPresentImage->SetBrush(SpriteBrush);
}

UWidgetAnimation* UPlayerInfoWidget::GetAnimationByName(FName& AnimName) const
{
	UWidgetAnimation*  const* WidgetAnim = AnimationsMap.Find(AnimName);
	if (WidgetAnim)
		return *WidgetAnim;

	return nullptr;
}

void UPlayerInfoWidget::FillAnimMap()
{
	AnimationsMap.Empty();

	for (TFieldIterator<FObjectProperty> PropIt(GetClass(), EFieldIteratorFlags::IncludeSuper); PropIt; ++PropIt)
	{
		FObjectProperty* Prop = *PropIt;
		if (Prop->PropertyClass == UWidgetAnimation::StaticClass())
		{
			UObject* Obj = Prop->GetObjectPropertyValue_InContainer(this);
			UWidgetAnimation* anim = Cast<UWidgetAnimation>(Obj);

			FName animName = anim->GetName();

			AnimationsMap.Add(animName, anim);
		}
	}

}
