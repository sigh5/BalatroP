// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/MainMenuWidget.h"

#include "Components/CanvasPanel.h"
/*
#include "Components/ProgressBar.h"
#include "Components\TextBlock.h"
#include "Components\Image.h"*/

#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/WidgetPool/BBUserWidgetPool.h"

#include "UI/View/BlindSelect/BlindSelectView.h"
#include "UI/View/PlayerInfoView/PlayerInfoWidget.h"
#include "UI/View/JokerView/JokerSlotWidget.h"
#include "UI/View/CardDeck/CardDeckView.h"
#include "UI/View/Store/StoreView.h"
#include "UI/View/Reward/RewardView.h"

UMainMenuWidget::UMainMenuWidget()
{
	ViewModelClass = UVM_MainMenu::StaticClass();
	ViewModelName = TEXT("VM_MainMenu");
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_MainMenu>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));
	VMInst->AddFieldValueChangedDelegate(UVM_MainMenu::FFieldNotificationClassDescriptor::CurWidgetName,
		FFieldValueChangedDelegate::CreateUObject(this, &UMainMenuWidget::VM_FieldChanged_WidgetName));

	WidgetPool = NewObject<UBBUserWidgetPool>(this);
	FName CurViewName = "";

	if (BlindSelectView == nullptr)
	{
		BlindSelectView = LoadClass<UBlindSelectView>(nullptr, TEXT("/Game/UI/View/SelectBlind/WBP_SelectBlind.WBP_SelectBlind_C"));
		CurViewName = "SelectBlindView";

		UBBUserWidgetBase* BlindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(BlindSelectView));
		if (BlindWidget)
		{
			BlindWidget->SetVisibility(ESlateVisibility::Visible);
			CanvasSlot->AddChildToCanvas(BlindWidget);
		}
	}

	if (PlayerInfoView == nullptr)
	{
		PlayerInfoView = LoadClass<UPlayerInfoWidget>(nullptr, TEXT("/Game/UI/View/PlayerInfo/WBP_PlayerInfo.WBP_PlayerInfo_C"));
		CurViewName = "PlayerInfoView";
		UBBUserWidgetBase* BlindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(PlayerInfoView));
		if (BlindWidget)
		{
			BlindWidget->SetVisibility(ESlateVisibility::Visible);
			CanvasSlot->AddChildToCanvas(BlindWidget);
		}
	}

	if (JokerSlotView == nullptr)
	{
		JokerSlotView = LoadClass<UJokerSlotWidget>(nullptr, TEXT("/Game/UI/View/Joker/WBP_JokerSlot.WBP_JokerSlot_C"));
		CurViewName = "JokerSlotView";
		UBBUserWidgetBase* BlindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(JokerSlotView));
		if (BlindWidget)
		{
			BlindWidget->SetVisibility(ESlateVisibility::Visible);
			CanvasSlot->AddChildToCanvas(BlindWidget);
		}
	}

	if (CadDeckView == nullptr)
	{
		CadDeckView = LoadClass<UCardDeckView>(nullptr, TEXT("/Game/UI/View/CardDeck/WBP_CardDeck.WBP_CardDeck_C"));
		CurViewName = "CadDeckView";
		UBBUserWidgetBase* BlindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(CadDeckView));
	}

	if (StoreView == nullptr)
	{
		StoreView = LoadClass<UStoreView>(nullptr, TEXT("/Game/UI/View/StoreView/WBP_Stroe.WBP_Stroe_C"));
		CurViewName = "StoreView";
		UBBUserWidgetBase* BlindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(StoreView));
	}

	if (RewardView == nullptr)
	{
		RewardView = LoadClass<URewardView>(nullptr, TEXT("/Game/UI/View/Reward/WBP_Reward.WBP_Reward_C"));
		CurViewName = "RewardView";
		UBBUserWidgetBase* BlindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(RewardView));
	}

}

void UMainMenuWidget::VM_FieldChanged_WidgetName(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_MainMenu>();
	check(VMInst);

	auto WidgetInfo = VMInst->GetCurWidgetName();


	UBBUserWidgetBase* CurWidget = WidgetPool->GetWidget(WidgetInfo.CurWidgetName);
	if (CurWidget)
	{
		WidgetPool->SetWidgetVisible(WidgetInfo.CurWidgetName, WidgetInfo.bActive);
		if (WidgetInfo.bActive)
		{
			CanvasSlot->AddChildToCanvas(CurWidget);
		}
		else
		{
			CurWidget->RemoveFromParent();
		}
	}
}
