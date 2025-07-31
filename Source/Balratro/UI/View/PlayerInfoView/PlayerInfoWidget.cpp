// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/PlayerInfoView/PlayerInfoWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

UPlayerInfoWidget::UPlayerInfoWidget()
{
	ViewModelClass = UVM_PlayerInfo::StaticClass();
	ViewModelName = TEXT("VM_PlayerInfo");
}

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

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

	FName VM_BlindSelct = "VM_BlindSelect";

	const auto VM_BlindSelect = TryGetViewModel<UVM_BlindSelect>(VM_BlindSelct, UVM_BlindSelect::StaticClass());
	checkf(IsValid(VM_BlindSelect), TEXT("Couldn't find a valid ViewModel"));

	VM_BlindSelect->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::SmallGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindGrade));

	VM_BlindSelect->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BigGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindGrade));

	VM_BlindSelect->AddFieldValueChangedDelegate(UVM_BlindSelect::FFieldNotificationClassDescriptor::BossGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &UPlayerInfoWidget::VM_FieldChanged_BlindGrade));

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

	ChipText->SetText(FText::AsNumber(VMInstance->GetCurChip()));
}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Level(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);


}

void UPlayerInfoWidget::VM_FieldChanged_CurHandRanking_Drainage(UObject* Object, UE::FieldNotification::FFieldId FieldId)
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

	MainOrderText->SetText(FText::FromName(VMInstance->GetMainOrder()));

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MainOrderText->Slot);
	check(CanvasSlot);
	FSlateFontInfo FontInfo = MainOrderText->Font;

	FVector2D CurrentPosition = CanvasSlot->GetPosition();
	if (VMInstance->GetBlindInfoActive())
	{
		CanvasSlot->SetPosition(FVector2D(MainOrderPos.X , MainOrderPos.Y - 40.f));
		FontInfo.Size = 40;
	}
	else
	{
		CanvasSlot->SetPosition(MainOrderPos);
		FontInfo.Size = 35;
	}

	MainOrderText->SetFont(FontInfo);
}

void UPlayerInfoWidget::VM_FieldChanged_BlindInfoActive(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_PlayerInfo>(Object);
	
	bool Flag = VMInstance->GetBlindInfoActive();
	if (Flag)
	{
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

}
