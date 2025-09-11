#include "UI/Button/JokerCard/JokerCardWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "UI/MVVM/ViewModel/VM_ItemSelect.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"


#include "PaperSprite.h"
#include "Styling/SlateBrush.h"

#include "Blueprint/WidgetBlueprintLibrary.h"  // DetectDragIfPressed, Drag&Drop 유틸리티
#include "Blueprint/DragDropOperation.h"       // UDragDropOperation

UJokerCardWidget::UJokerCardWidget()
{
	ViewModelClass = UVM_JockerSlot::StaticClass();
	ViewModelName = TEXT("VM_JockerSlot");
}

void UJokerCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJokerCardWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

FReply UJokerCardWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		DragOffset = LocalMousePos; // 클릭한 위치와 위젯 좌상단 차이

		return UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	bIsDragging = false;
	return Reply;
}

void UJokerCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	bIsDragging = true;

	DragVisual->ForceSwapData(JokerData);
	
	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->DefaultDragVisual = DragVisual->MainImage;
	DragOp->Payload = this;
	DragOp->Pivot = EDragPivot::MouseDown;
	OutOperation = DragOp;

	MainImage->SetRenderOpacity(0.f);
}

bool UJokerCardWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	MainImage->SetRenderOpacity(1.f);

	bIsDragging = false;

	if (InOperation && InOperation->Payload)
	{
		if (UJokerCardWidget* DraggedWidget = Cast<UJokerCardWidget>(InOperation->Payload))
		{
			if (DraggedWidget->JokerData != JokerData)
			{
				const auto VM = TryGetViewModel<UVM_JockerSlot>(); check(VM);
				auto SwapData0 = DraggedWidget->JokerData;
				
				DraggedWidget->ForceSwapData(this->JokerData);
				DraggedWidget->IsSelected = IsSelected;

				ForceSwapData(SwapData0);
	
				if (DraggedWidget->IsSelected != IsSelected)
				{
					DraggedWidget->ForceSwapPos();
					ForceSwapPos();
				}

				VM->SwapCardData(DraggedWidget->JokerData, JokerData);
			}

			DraggedWidget->MainImage->SetRenderOpacity(1.f);

			return true;
		}
	}

	return false;
}

FReply UJokerCardWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (!bIsDragging)
		{
			OnJokerButtonClicked();
		}
		MainImage->SetRenderOpacity(1.f);
	}

	bIsDragging = false;
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UJokerCardWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	MainImage->SetRenderOpacity(1.f);
	
	bIsDragging = false;
}



void UJokerCardWidget::SetInit()
{
	IsSelected = false;
	
	EventButton->OnClicked.AddDynamic(this, &UJokerCardWidget::OnSellButtonClicked);
}

void UJokerCardWidget::SetInfo(UJokerCard_Info* _inValue)
{
	JokerData = _inValue;

	if (IsCreated == false)
	{
		CreateJokerImage();
		DragVisual = CreateWidget<UJokerCardWidget>(GetWorld(), GetClass());
		DragVisual->bIsDummyData = true;
	}
	else
		ChangeJokerImage();

	IsSelected = false;
	SetVisibility(ESlateVisibility::Visible); // 모든 버튼 보이게 초기화

	EventButton->SetVisibility(ESlateVisibility::Collapsed);

	if (IsCreated == false)
		IsCreated = true;
}

void UJokerCardWidget::ShakingEvent()
{
	PlayAnimation(ShakingAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

}

void UJokerCardWidget::PlayJokerEvent(UTextBlock* SkillText0, UTextBlock* SkillText1)
{
	if (SkillText0 == nullptr || SkillText1 == nullptr)
		return;

	ShakingEvent();

	FString ScoreStr = "";
	FString ScoreStr2 = "";

	if (JokerData->Info.JokerType == EJokerType::SPADE)
	{
		ScoreStr = FString::Printf(TEXT("+%d Draiage"), 4);
	}
	else if (JokerData->Info.JokerType == EJokerType::HEART_MUL)
	{
		ScoreStr = FString::Printf(TEXT("x%.1f Draiage"), 1.5f);
	}
	else if (JokerData->Info.JokerType == EJokerType::ACE_PLUS)
	{
		ScoreStr = FString::Printf(TEXT("+%d Draiage"), 4);
		ScoreStr2 = FString::Printf(TEXT("+%d Chip"), 30);
	}

	SkillText0->SetText(FText::FromString(ScoreStr));
	SkillText1->SetText(FText::FromString(ScoreStr2));
}

void UJokerCardWidget::SetCopyJoker(EJokerType JokerType)
{
	JokerData->Info.JokerType = JokerType;
}

void UJokerCardWidget::OnSellButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_JockerSlot>(); check(VMInst);

	IsStore = !IsStore;
	SetVisibility(ESlateVisibility::Collapsed);
	
	VMInst->SetAddJokerCard(JokerData->Info);
}

void UJokerCardWidget::OnButtonHover()
{

}

void UJokerCardWidget::ForceSwapData(UJokerCard_Info* InfoData)
{
	JokerData = InfoData;

	if (UPaperSprite* Sprite = JokerData->Info.CardSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(126.f, 186.f); // 원하는 크기
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImage->SetBrush(SpriteBrush);
	}

}

void UJokerCardWidget::SetIsStoreHave(bool _Selected)
{
	IsStore = _Selected;

	if (IsStore)
	{
		BuyBorder->SetVisibility(ESlateVisibility::Visible);
		BuyPriceText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BuyBorder->SetVisibility(ESlateVisibility::Collapsed);
		BuyPriceText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UJokerCardWidget::OnJokerButtonClicked()
{
	IsSelected = !IsSelected;


	if (IsStore)
	{
		UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
		FMargin Margin = HSlot->GetPadding();

		if (IsSelected)
		{
			Margin.Top -= 50.f;  // 선택한 카드 내리는 것
			EventButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Margin.Top += 50.f;  // 선택한 카드 올리는 것
			EventButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		HSlot->SetPadding(Margin);
	}
	else
	{
		UWrapBoxSlot* HSlot = Cast<UWrapBoxSlot>(Slot);
		FMargin Margin = HSlot->GetPadding();

		if (IsSelected)
		{
			Margin.Top -= 25.f;  // 선택한 카드 내리는 것
			EventButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Margin.Top += 25.f;  // 선택한 카드 올리는 것
			EventButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		HSlot->SetPadding(Margin);
	}

	FString Pricestr = FString::Printf(TEXT("$%d"), JokerData->Info.Price);
	PriceText->SetText(FText::FromString(Pricestr));

}

void UJokerCardWidget::ChangeJokerImage()
{
	if (UPaperSprite* Sprite = JokerData->Info.CardSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(126.f, 186.f); // 원하는 크기
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImage->SetBrush(SpriteBrush);
	}

	FString Pricestr = FString::Printf(TEXT("$%d"), JokerData->Info.Price);
	PriceText->SetText(FText::FromString(Pricestr));
	//BuyPriceText->
}

void UJokerCardWidget::CreateJokerImage()
{
	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);

	ChangeJokerImage();
	SetInit();
}


void UJokerCardWidget::ForceSwapPos()
{
	UWrapBoxSlot* HSlot = Cast<UWrapBoxSlot>(Slot);
	check(HSlot);
	if (IsSelected == false)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top += 100.f;  // 선택한 카드 내리는 것
		HSlot->SetPadding(Margin);
	}
	else if (IsSelected == true)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 100.f;
		HSlot->SetPadding(Margin);
	}
}
