// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/JokerView/JokerSlotWidget.h"

#include <Components/TextBlock.h>
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/CanvasPanelSlot.h"

#include "UI/MVVM/ViewModel/VM_JockerSlot.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "UI/Button/JokerCard/JokerCardWidget.h"


UJokerSlotWidget::UJokerSlotWidget()
{
	ViewModelClass = UVM_JockerSlot::StaticClass();
	ViewModelName = TEXT("VM_JockerSlot");
}

void UJokerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJokerSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	JokerCardSubClass = LoadClass<UJokerCardWidget>(nullptr, TEXT("/Game/UI/View/Joker/WBP_JokerCard.WBP_JokerCard_C"));

	const auto VMInst = TryGetViewModel<UVM_JockerSlot>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::JokerDatas,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_AddJokerCard));

	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::CalculatorFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_CalcualtorJokerCard));

	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::PlayEventJoker,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_PlayJokerEvent));

	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::JokerEventStopFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_PreJokerEventStopFlag));

	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::RoundFinishEventJoker,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_RoundFinishEvent));

	SkillText->SetVisibility(ESlateVisibility::Collapsed);
	SkillText2->SetVisibility(ESlateVisibility::Collapsed);
}

void UJokerSlotWidget::VM_FieldChanged_AddJokerCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object); check(VMInstance);
	auto& JokerDatas = VMInstance->GetJokerDatas();
	int32 DataNums = JokerDatas.Num();

	for (int i = 0; i < DataNums; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), static_cast<int>(JokerDatas[i]->Info.JokerType));

		UJokerCardWidget* NewButton = ReuseCardButtonWidget(DataNums, i, JokerDatas[i]);
		UWrapBoxSlot* BoxSlot = Cast<UWrapBoxSlot>(JokerWrapBox->AddChild(NewButton));
		if (BoxSlot)
		{
			BoxSlot->SetHorizontalAlignment(HAlign_Center);
			BoxSlot->SetVerticalAlignment(VAlign_Center);
			BoxSlot->SetPadding(FMargin(10.0f, 10.0f));
			BoxSlot->SetFillEmptySpace(false);  // true로 하면 빈 공간까지 채워서 늘어남
		}
	}

	JokerWrapBox->SetHorizontalAlignment(HAlign_Center);
}

void UJokerSlotWidget::VM_FieldChanged_CalcualtorJokerCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object); check(VMInstance);
	auto& JokerDatas = VMInstance->GetJokerDatas();

	TimerFuncQueue.Empty();
	GetWorld()->GetTimerManager().ClearTimer(FinishJokerTimerHandle);

	JokerScroe_EffectText();
}

void UJokerSlotWidget::VM_FieldChanged_PlayJokerEvent(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object); check(VMInstance);
	auto CurPreJokerType = VMInstance->GetPlayEventJoker();

	GetWorld()->GetTimerManager().ClearTimer(JokerEffectTimerHandle);

	if (CurPreJokerType == EJokerType::NONE)
		return;

	int32 CopyJokerIndex = -1;

	if (CurPreJokerType == EJokerType::COPY)
	{
		for (int32 i=0; i<JokerButtons.Num(); ++i)
		{
			if (JokerButtons[i]->GetInfo()->Info.JokerType == CurPreJokerType)
			{
				CopyJokerIndex = i;
				break;
			}
		}

		if (CopyJokerIndex != -1 &&
			CopyJokerIndex +1 < JokerButtons.Num() &&
			JokerButtons[CopyJokerIndex + 1]->IsVisible())
		{
			CopyJokerIndex += 1;
		}

	}


	for (auto JokerCard : JokerButtons)
	{
		if (CurPreJokerType == EJokerType::COPY && JokerCard->GetInfo()->Info.JokerType == CurPreJokerType)
		{
			if (CopyJokerIndex == -1)
				return;

			JokerCard->SetCopyJoker(JokerButtons[CopyJokerIndex]->GetInfo()->Info.JokerType);
			JokerCard->PlayJokerEvent(SkillText, SkillText2);
			SetSkillTextPos(JokerCard);
			SetSkillTextPos2(JokerCard);
			JokerCard->SetCopyJoker(EJokerType::COPY);
			break;
		}
		else if (JokerCard->GetInfo()->Info.JokerType == CurPreJokerType)
		{
			JokerCard->PlayJokerEvent(SkillText, SkillText2);
			SetSkillTextPos(JokerCard);
			SetSkillTextPos2(JokerCard);
			break;
		}
	}
}

void UJokerSlotWidget::VM_FieldChanged_PreJokerEventStopFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object); check(VMInstance);

	if (VMInstance->GetJokerEventStopFlag() == false)
		return;

	SkillText->SetVisibility(ESlateVisibility::Collapsed);
	SkillText2->SetVisibility(ESlateVisibility::Collapsed);
}


UJokerCardWidget* UJokerSlotWidget::ReuseCardButtonWidget(int32 AllNum, int32 Index, UJokerCard_Info* Data)
{
	UJokerCardWidget* NewButton = nullptr;
	if (JokerButtons.Num() < AllNum)
	{
		NewButton = CreateWidget<UJokerCardWidget>(this, JokerCardSubClass);
		JokerButtons.Add(NewButton);
	}
	else
	{
		NewButton = JokerButtons[Index];
	}

	check(NewButton);
	NewButton->SetInfo(Data);
	NewButton->SetIsStoreHave(false);

	return NewButton;
}

void UJokerSlotWidget::JokerScroe_EffectText()
{
	const auto VMInst = TryGetViewModel<UVM_JockerSlot>();
	auto& Data = VMInst->GetJokerDatas();

	for (int i = 0; i < Data.Num(); ++i)
	{
		for (auto& Joker : JokerButtons)
		{
			if (Joker->GetInfo()->Info.JokerType == Data[i]->Info.JokerType)
			{
				FJokerStat CurData = Data[i]->Info;
				SetJoker_EffectOrder(Joker, CurData);
				break;
			}
		}
	}

	StartNextTimer();
}

void UJokerSlotWidget::SetJoker_EffectOrder(UJokerCardWidget* EventJoker, FJokerStat& JokerData)
{
	if (EventJoker)
	{
		auto VM = TryGetViewModel<UVM_JockerSlot>(); check(VM);

		if (JokerData.JokerType == EJokerType::BASE_JOKER)
		{
			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					UE_LOG(LogTemp, Warning, TEXT("EJokerType::BASE_JOKER"));

					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					FString ScoreStr = FString::Printf(TEXT("+%d Draiage"), 4);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
					VM_PlayerInfo->SetCurDrainage(CurDrainage + 4);

				
					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::LAST_HAND_MUL3)
		{
			if (VM->GetIsLastHandPlay() == false)
				return;

			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					FString ScoreStr = FString::Printf(TEXT("x%d Draiage"), 3);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
					VM_PlayerInfo->SetCurDrainage(CurDrainage * 3);

					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::PAIR_MUL_DRANGE)
		{
			if (VM->GetCurrentPokerHand() != EPokerHand::ONE_PAIR)
				return;

			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					FString ScoreStr = FString::Printf(TEXT("x%d Draiage"), 2);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
					VM_PlayerInfo->SetCurDrainage(CurDrainage * 2);

					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::PAIR_CHIP)
		{
			if (VM->GetCurrentPokerHand() != EPokerHand::ONE_PAIR)
				return;

			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					FString ScoreStr = FString::Printf(TEXT("+%d Chips"), 30);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurChip = VM_PlayerInfo->GetCurChip();
					VM_PlayerInfo->SetCurDrainage(CurChip + 30);

					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::PAIR_DRAINAGE)
		{
			if (VM->GetCurrentPokerHand() != EPokerHand::ONE_PAIR)
				return;

			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					FString ScoreStr = FString::Printf(TEXT("+%d Draiage"), 4);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
					VM_PlayerInfo->SetCurDrainage(CurDrainage + 4);

					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::JOKER_GOLD_SUM)
		{
			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					auto VM_CardDeck = TryGetViewModel<UVM_JockerSlot>(); check(VM_CardDeck);

					int32 JokerSum = static_cast<int32>(VM_CardDeck->GetAddtionalValue());

					FString ScoreStr = FString::Printf(TEXT("+%d Draiage"), JokerSum);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
					VM_PlayerInfo->SetCurDrainage(CurDrainage + JokerSum);

				
					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::POP_CONE)
		{
			PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					auto VM_CardDeck = TryGetViewModel<UVM_JockerSlot>(); check(VM_CardDeck);

					float BaseDrainage = VM_CardDeck->GetAddtionalValue();

					FString ScoreStr = FString::Printf(TEXT("x%.2f Draiage"), BaseDrainage);
					SkillText->SetText(FText::FromString(ScoreStr));
					SetSkillTextPos(CurEventCard);

					int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
					VM_PlayerInfo->SetCurDrainage(CurDrainage * BaseDrainage);

					

					CurEventCard->ShakingEvent();

				}, EventJoker, JokerData);
		}
		else if (JokerData.JokerType == EJokerType::COPY)
		{
			auto VM_CardDeck= TryGetViewModel<UVM_JockerSlot>(); check(VM_CardDeck);

			if (VM_CardDeck->GetCopyJokerSetting() == EJokerType::NONE)
				return;

			FJokerStat Temp = JokerData;

			Temp.JokerType = VM_CardDeck->GetCopyJokerSetting();

			SetJoker_EffectOrder(EventJoker, Temp);
		}
	}
}

void UJokerSlotWidget::PushTimerEvent(TFunction<void(class UJokerCardWidget*, FJokerStat)> InFunc, UJokerCardWidget* CurEventJoker, FJokerStat InValue)
{
	FTimerDelegate Delegate;
	Delegate.BindLambda([this, InFunc, CurEventJoker, InValue]()
		{
			// InValue를 그대로 전달해 실행
			InFunc(CurEventJoker, InValue);

			// 다음 이벤트 실행
			StartNextTimer();
		});
	TimerFuncQueue.Enqueue(Delegate);
}

void UJokerSlotWidget::SetSkillTextPos(UJokerCardWidget* CurEventCard)
{
	SkillText->SetVisibility(ESlateVisibility::HitTestInvisible);

	FGeometry CardGeo = CurEventCard->GetCachedGeometry();
	FVector2D AbsPos = CardGeo.GetAbsolutePosition();

	if (UWidget* CanvasParent = SkillText->GetParent())
	{
		FGeometry ParentGeo = CanvasParent->GetCachedGeometry();
		FVector2D LocalCenter = ParentGeo.AbsoluteToLocal(AbsPos);

		LocalCenter.Y -= 20.f;

		if (UCanvasPanelSlot* MySlot = Cast<UCanvasPanelSlot>(SkillText->Slot))
		{
			MySlot->SetPosition(LocalCenter);
		}
	}
}

void UJokerSlotWidget::SetSkillTextPos2(UJokerCardWidget* CurEventCard)
{
	SkillText2->SetVisibility(ESlateVisibility::HitTestInvisible);

	FGeometry CardGeo = CurEventCard->GetCachedGeometry();
	FVector2D AbsPos = CardGeo.GetAbsolutePosition();

	if (UWidget* CanvasParent = SkillText2->GetParent())
	{
		FGeometry ParentGeo = CanvasParent->GetCachedGeometry();
		FVector2D LocalCenter = ParentGeo.AbsoluteToLocal(AbsPos);

		LocalCenter.Y -= 55.f;


		if (UCanvasPanelSlot* MySlot = Cast<UCanvasPanelSlot>(SkillText2->Slot))
		{
			MySlot->SetPosition(LocalCenter);
		}
	}
}

void UJokerSlotWidget::StartNextTimer()
{
	auto VM_CardDeck = TryGetViewModel<UVM_CardDeck>("VM_CardDeck", UVM_CardDeck::StaticClass());
	check(VM_CardDeck);

	if (VM_CardDeck->GetIsHandPlayFlag() == false)
	{
		TimerFuncQueue.Empty();
	}

	if (TimerFuncQueue.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(JokerEffectTimerHandle);
	
		FTimerDelegate FinishDelegate;
		FinishDelegate.BindLambda([&]() {
			SkillText->SetVisibility(ESlateVisibility::Collapsed);
			SkillText2->SetVisibility(ESlateVisibility::Collapsed);
			SkillText->SetText(FText::FromString(""));
			SkillText2->SetText(FText::FromString(""));
			});

		GetWorld()->GetTimerManager().SetTimer(
			FinishJokerTimerHandle,
			FinishDelegate,
			0.5f,
			false);
		
		auto VM_Joker = TryGetViewModel<UVM_JockerSlot>(); check(VM_Joker);
		VM_Joker->All_EffectFinish();

		return;
	}

	FTimerDelegate Delegate;
	TimerFuncQueue.Dequeue(Delegate);

	GetWorld()->GetTimerManager().SetTimer(
		JokerEffectTimerHandle,
		Delegate,
		0.5f,
		false);
}


void UJokerSlotWidget::VM_FieldChanged_RoundFinishEvent(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_JockerSlot>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	for (auto Joker : JokerButtons)
	{
		if (VMInst->GetRoundFinishEventJoker() != Joker->GetInfo()->Info.JokerType)
			continue;

		FString ScoreStr = FString::Printf(TEXT("+%d price"), 3);
		SkillText->SetText(FText::FromString(ScoreStr));
		SetSkillTextPos(Joker);
	}

	FTimerDelegate Delegate;
	Delegate.BindLambda([&]()
		{
			SkillText->SetText(FText::FromString(""));
			SkillText->SetVisibility(ESlateVisibility::Collapsed);
		});

	GetWorld()->GetTimerManager().SetTimer(
		JokerEffectTimerHandle,
		Delegate,
		1.5f,
		false);


}