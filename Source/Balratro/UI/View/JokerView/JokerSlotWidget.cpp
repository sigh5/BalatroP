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

#include "UI/Button/JokerCard/JokerCardWidget.h"


UJokerSlotWidget::UJokerSlotWidget()
{
	ViewModelClass = UVM_JockerSlot::StaticClass();
	ViewModelName = TEXT("VM_JockerSlot");
}

void UJokerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	JokerCardSubClass = LoadClass<UJokerCardWidget>(nullptr, TEXT("/Game/UI/View/Joker/WBP_JokerCard.WBP_JokerCard_C"));

	const auto VMInst = TryGetViewModel<UVM_JockerSlot>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::JokerDatas,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_AddJokerCard));
	
	VMInst->AddFieldValueChangedDelegate(UVM_JockerSlot::FFieldNotificationClassDescriptor::CalculatorFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UJokerSlotWidget::VM_FieldChanged_CalcualtorJokerCard));

	SkillText->SetVisibility(ESlateVisibility::Collapsed);
}

void UJokerSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

void UJokerSlotWidget::VM_FieldChanged_AddJokerCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_JockerSlot>(Object); check(VMInstance);
	auto& JokerDatas = VMInstance->GetJokerDatas();
	int32 DataNums = JokerDatas.Num();

	for (int i = 0; i < DataNums; ++i)
	{
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
	NewButton->SetInfo(Data->Info);
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
			if (Joker->GetInfo() == Data[i]->Info)
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
		PushTimerEvent([&](UJokerCardWidget* CurEventCard, FJokerStat Value)
			{
				auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
				check(VM_PlayerInfo);

				FString ScoreStr = FString::Printf(TEXT("+%d Draiage"), 4);
				SkillText->SetText(FText::FromString(ScoreStr));
				SetSkillTextPos(CurEventCard);

				int32 CurDrainage = VM_PlayerInfo->GetCurDrainage();
				VM_PlayerInfo->SetCurDrainage(CurDrainage + 4);

				SkillText->SetVisibility(ESlateVisibility::HitTestInvisible);

				CurEventCard->ShakingEvent();

			}, EventJoker, JokerData);
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
	FGeometry CardGeo = CurEventCard->GetCachedGeometry();
	FVector2D AbsPos = CardGeo.GetAbsolutePosition();
	FVector2D Size = CardGeo.GetLocalSize();

	if (UWidget* CanvasParent = SkillText->GetParent())
	{
		FGeometry ParentGeo = CanvasParent->GetCachedGeometry();
		FVector2D LocalCenter = ParentGeo.AbsoluteToLocal(AbsPos);

		//LocalCenter.X -= .f;

		UE_LOG(LogTemp, Warning, TEXT("SetSkillTextPos"));

		if (UCanvasPanelSlot* MySlot = Cast<UCanvasPanelSlot>(SkillText->Slot))
		{
			MySlot->SetPosition(LocalCenter);
		}
	}
}

void UJokerSlotWidget::StartNextTimer()
{
	if (TimerFuncQueue.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(JokerEffectTimerHandle);

		FTimerDelegate FinishDelegate;
		FinishDelegate.BindLambda([&]() {
			SkillText->SetVisibility(ESlateVisibility::Collapsed);
			SkillText->SetText(FText::FromString(""));
			});

		GetWorld()->GetTimerManager().SetTimer(
			FinishJokerTimerHandle,
			FinishDelegate,
			0.5f,
			false);
		
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
