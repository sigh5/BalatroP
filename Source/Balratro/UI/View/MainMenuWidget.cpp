// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/MainMenuWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/Border.h"

#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/WidgetPool/BBUserWidgetPool.h"

#include "UI/View/BlindSelect/BlindSelectView.h"
#include "UI/View/PlayerInfoView/PlayerInfoWidget.h"
#include "UI/View/JokerView/JokerSlotWidget.h"
#include "UI/View/CardDeck/CardDeckView.h"
#include "UI/View/Store/StoreView.h"
#include "UI/View/Reward/RewardView.h"
#include "UI/View/ItemSelect/ItemSelectView.h"
#include "UI/View/HandRanking/HandRankingView.h"
#include "UI/View/GameOver/GameOverView.h"

// 디버그일떄만
#include "Core/MyPlayerState.h"

UMainMenuWidget::UMainMenuWidget()
{
	ViewModelClass = UVM_MainMenu::StaticClass();
	ViewModelName = TEXT("VM_MainMenu");
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_MainMenu>(); checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));
	
	VMInst->AddFieldValueChangedDelegate(UVM_MainMenu::FFieldNotificationClassDescriptor::CurWidgetName,
		FFieldValueChangedDelegate::CreateUObject(this, &UMainMenuWidget::VM_FieldChanged_WidgetName));
	
	VMInst->AddFieldValueChangedDelegate(UVM_MainMenu::FFieldNotificationClassDescriptor::ClearFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UMainMenuWidget::VM_FieldChanged_ClearAnimFlag));

	WidgetPool = NewObject<UBBUserWidgetPool>(this);
	FName CurViewName = "";

	if (BlindSelectView == nullptr)
	{
		BlindSelectView = LoadClass<UBlindSelectView>(nullptr, TEXT("/Game/UI/View/SelectBlind/WBP_SelectBlind.WBP_SelectBlind_C"));
		CurViewName = "SelectBlindView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(BlindSelectView));
		
		UBlindSelectView* BlindSelect = Cast<UBlindSelectView>(BindWidget);
		BlindSelect->SetIsToolTipView(false);

		if (BindWidget)
		{
#ifdef Bline_View_TEST
			BindWidget->SetVisibility(ESlateVisibility::Visible);
#else
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
#endif
			CanvasSlot->AddChildToCanvas(BindWidget);
		}
	}

	if (PlayerInfoView == nullptr)
	{
		PlayerInfoView = LoadClass<UPlayerInfoWidget>(nullptr, TEXT("/Game/UI/View/PlayerInfo/WBP_PlayerInfo.WBP_PlayerInfo_C"));
		CurViewName = "PlayerInfoView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(PlayerInfoView));
		
		if (BindWidget)
		{
#ifdef PlayerInfoView_VIEW_TEST
			BindWidget->SetVisibility(ESlateVisibility::Visible);
#else
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
#endif
			CanvasSlot->AddChildToCanvas(BindWidget);
		}
	}

	if (JokerSlotView == nullptr)
	{
		JokerSlotView = LoadClass<UJokerSlotWidget>(nullptr, TEXT("/Game/UI/View/Joker/WBP_JokerSlot.WBP_JokerSlot_C"));
		CurViewName = "JokerSlotView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(JokerSlotView));
	
		if (BindWidget)
		{
			CanvasSlot->AddChildToCanvas(BindWidget);
#ifdef JokerSlotView_VIEW_TEST
			BindWidget->SetVisibility(ESlateVisibility::Visible);
#else
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
#endif
		}
	}

	if (CadDeckView == nullptr)
	{
		CadDeckView = LoadClass<UCardDeckView>(nullptr, TEXT("/Game/UI/View/CardDeck/WBP_CardDeck.WBP_CardDeck_C"));
		CurViewName = "CardDeckView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(CadDeckView));
		

		if (BindWidget)
		{
			CanvasSlot->AddChildToCanvas(BindWidget);
#ifdef 	CadDeckView_View_TEST
			BindWidget->SetVisibility(ESlateVisibility::Visible);
#else
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
#endif
		}

	}

	if (StoreView == nullptr)
	{
		StoreView = LoadClass<UStoreView>(nullptr, TEXT("/Game/UI/View/StoreView/WBP_Stroe.WBP_Stroe_C"));
		CurViewName = "StoreView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(StoreView));
	
		if (BindWidget)
		{
			CanvasSlot->AddChildToCanvas(BindWidget);
#ifdef Store_View_TEST
		BindWidget->SetVisibility(ESlateVisibility::Visible);
#else
		BindWidget->SetVisibility(ESlateVisibility::Collapsed);
#endif
		}
	}

	if (RewardView == nullptr)
	{
		RewardView = LoadClass<URewardView>(nullptr, TEXT("/Game/UI/View/Reward/WBP_Reward.WBP_Reward_C"));
		CurViewName = "RewardView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(RewardView));
		
		if (BindWidget)
		{
			CanvasSlot->AddChildToCanvas(BindWidget);
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (ItemSelectView == nullptr)
	{
		ItemSelectView = LoadClass<UItemSelectView>(nullptr, TEXT("/Game/UI/View/ItemSelectView/WBP_ItemSelect.WBP_ItemSelect_C"));
		CurViewName = "ItemSelectView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(ItemSelectView));
		
		if (BindWidget)
		{
			CanvasSlot->AddChildToCanvas(BindWidget);
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (HandRankingView == nullptr)
	{
		HandRankingView = LoadClass<UHandRankingView>(nullptr, TEXT("/Game/UI/View/HandRanking/WBP_HandRanking.WBP_HandRanking_C"));
		CurViewName = "HandRankingView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(HandRankingView));

		if (BindWidget)
		{
#ifdef HandRankingView_View_TEST
			BindWidget->SetVisibility(ESlateVisibility::Visible);
#else
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
#endif
			CanvasSlot->AddChildToCanvas(BindWidget);
		}
	}

	if (GameOverView == nullptr)
	{
		GameOverView = LoadClass<UGameOverView>(nullptr, TEXT("/Game/UI/View/GameOver/WBP_GameOver.WBP_GameOver_C"));
		CurViewName = "GameOverView";
		UBBUserWidgetBase* BindWidget = WidgetPool->AddWidget(this, CurViewName, TSubclassOf<UBBUserWidgetBase>(GameOverView));
		if (BindWidget)
		{
			CanvasSlot->AddChildToCanvas(BindWidget);
			BindWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}


	
}

void UMainMenuWidget::VM_FieldChanged_WidgetName(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_MainMenu>(); check(VMInst);
	auto WidgetInfo = VMInst->GetCurWidgetName();

	UBBUserWidgetBase* CurWidget = WidgetPool->GetWidget(WidgetInfo.CurWidgetName);
	if (CurWidget)
	{
		WidgetPool->SetWidgetVisible(WidgetInfo.CurWidgetName, WidgetInfo.bActive);
	}
}

void UMainMenuWidget::VM_FieldChanged_ClearAnimFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_MainMenu>();
	check(VMInst);
	
	PlayAnimation(ClearAnimation);
}
