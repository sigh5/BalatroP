#include "UI/Button/Card/CardButtonWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"


#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"


#include "Components/Widget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"  // DetectDragIfPressed, Drag&Drop 유틸리티
#include "Blueprint/DragDropOperation.h"       // UDragDropOperation


UCardButtonWidget::UCardButtonWidget()
{
	ViewModelName = TEXT("VM_CardDeck");
	ViewModelClass = UVM_CardDeck::StaticClass();
}

void UCardButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (bIsDummyData == true)
	{
		return;
	}

	Ghost_Image->SetVisibility(ESlateVisibility::Collapsed);
	Foil_Image->SetVisibility(ESlateVisibility::Collapsed);

	FString AssetPath = TEXT("/Game/CardResuorce/Enhancers/MainDeck.MainDeck");
	TSoftObjectPtr<UPaperSprite> Sprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Sprite.IsValid())
	{
		Sprite.LoadSynchronous();
	}

	FSlateBrush SpriteBrush;
	SpriteBrush.SetResourceObject(Sprite.Get());
	SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
	SpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
	MainImage->SetBrush(SpriteBrush);

	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	//EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);

	EndDelegate.Clear();
	EndDelegate.BindDynamic(this, &UCardButtonWidget::OnFilpAnimationFinished);
	BindToAnimationFinished(FilpAnim, EndDelegate);
}

FReply UCardButtonWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UCardButtonWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	bIsDragging = true;

	DragVisual->ForceSwapData(this->CardInfoData);
	DragVisual->CardIndex = CardIndex;

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->DefaultDragVisual = DragVisual;
	DragOp->Payload = this;
	DragOp->Pivot = EDragPivot::MouseDown;
	OutOperation = DragOp;

	MainImage->SetRenderOpacity(0.f);
	Enhance_Image->SetRenderOpacity(0.f);
}

bool UCardButtonWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	MainImage->SetRenderOpacity(1.f);
	Enhance_Image->SetRenderOpacity(1.f);

	bIsDragging = false;

	if (InOperation && InOperation->Payload)
	{
		if (UCardButtonWidget* DraggedWidget = Cast<UCardButtonWidget>(InOperation->Payload))
		{
			if (DraggedWidget->CardInfoData->Info != CardInfoData->Info)
			{
				const auto VM = TryGetViewModel<UVM_CardDeck>(); check(VM);

				auto SwapData0 = DraggedWidget->CardInfoData;
				auto SwapCardIndex = DraggedWidget->CardIndex;
				auto SwapSelected = DraggedWidget->bSelected;

				DraggedWidget->ForceSwapData(CardInfoData);
				DraggedWidget->CardIndex = CardIndex;
				DraggedWidget->bSelected = bSelected;

				ForceSwapData(SwapData0);
				CardIndex = SwapCardIndex;
				bSelected = SwapSelected;

				if (DraggedWidget->bSelected != bSelected)
				{
					DraggedWidget->ForceSwapPos();
					ForceSwapPos();
				}

				VM->SwapCardData(DraggedWidget->CardInfoData, CardInfoData);
			}



			DraggedWidget->MainImage->SetRenderOpacity(1.f);
			DraggedWidget->Enhance_Image->SetRenderOpacity(1.f);

			return true;
		}
	}



	return false;
}

FReply UCardButtonWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (!bIsDragging)
		{
			OnCardButtonClicked();
		}
		MainImage->SetRenderOpacity(1.f);
		Enhance_Image->SetRenderOpacity(1.f);
	}

	bIsDragging = false;
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}


void UCardButtonWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	MainImage->SetRenderOpacity(1.f);
	Enhance_Image->SetRenderOpacity(1.f);


	bIsDragging = false;
}

void UCardButtonWidget::MoveAnimmation()
{
	PlayAnimation(MoveAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCardButtonWidget::ShakeAnimation()
{
	PlayAnimation(ShakeAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCardButtonWidget::DrawAnimation()
{
	PlayAnimation(MoveEndAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCardButtonWidget::ChuckAnimation()
{
	PlayAnimation(ChunkAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCardButtonWidget::OnCardButtonClicked()
{
	const auto VM = TryGetViewModel<UVM_CardDeck>(); check(VM);

	bool CurSelectedMax = VM->GetIsSelectedMax();

	bIsDragging = false;
	bSelected = !bSelected; // Toggle

	if (CurSelectedMax == true && bSelected == true) // 5장이 꽉찼고 추가적으로 더 선택할 경우 return;
	{
		bSelected = false;
		return;
	}

	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	check(HSlot);
	if (bSelected == false)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top += 100.f;  // 선택한 카드 내리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
	else if (bSelected == true && CurSelectedMax == false)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 100.f;    // 선택한 카드 올리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
}

void UCardButtonWidget::SetInfo(UHandInCard_Info* _inValue)
{
	CardInfoData = _inValue;

	MainButton->SetRenderOpacity(1.0f);

	if (IsCreated == false)
	{
		CreateImage();

		DragVisual = CreateWidget<UCardButtonWidget>(GetWorld(), GetClass());
		DragVisual->bIsDummyData = true;
	}
	else
	{
		ChangeImage();
	}

	if (IsCreated == false)
	{
		IsCreated = true;
	}

	PlayAnimation(FilpAnim);
}


void UCardButtonWidget::ChangeImage()
{
	LoadEnhanceImage();

	if (UPaperSprite* Sprite = CardInfoData->Info.CardSprite.Get())
	{
		MainImageSpriteBrush.SetResourceObject(Sprite);
		MainImageSpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImageSpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		MainImage->SetBrush(MainImageSpriteBrush);

	}

	if (UPaperSprite* Sprite = CardInfoData->Info.EnforceSprite.Get())
	{
		EnhanceImageSpriteBrush.SetResourceObject(Sprite);
		EnhanceImageSpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		EnhanceImageSpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		Enhance_Image->SetBrush(EnhanceImageSpriteBrush);
	}

	SetGoadEvent();
}

void UCardButtonWidget::CreateImage()
{
	ChangeImage();
	bSelected = false;
}

void UCardButtonWidget::ForceSwapData(UHandInCard_Info* CardInfoData0)
{
	CardInfoData = CardInfoData0;

	LoadEnhanceImage();

	if (UPaperSprite* Sprite = CardInfoData->Info.CardSprite.Get())
	{
		MainImageSpriteBrush.SetResourceObject(Sprite);
		MainImageSpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImageSpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		MainImage->SetBrush(MainImageSpriteBrush);
	}

	if (UPaperSprite* Sprite = CardInfoData->Info.EnforceSprite.Get())
	{
		EnhanceImageSpriteBrush.SetResourceObject(Sprite);
		EnhanceImageSpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		EnhanceImageSpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		Enhance_Image->SetBrush(EnhanceImageSpriteBrush);
	}

	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	//EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);
	Ghost_Image->SetVisibility(ESlateVisibility::Collapsed);
	Foil_Image->SetVisibility(ESlateVisibility::Collapsed);


}

void UCardButtonWidget::ForceSwapPos()
{
	auto VM = TryGetViewModel<UVM_CardDeck>(); check(VM);
	bool CurSelectedMax = VM->GetIsSelectedMax();

	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	check(HSlot);
	if (bSelected == false)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top += 100.f;  // 선택한 카드 내리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
	else if (bSelected == true)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 100.f;
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
}

void UCardButtonWidget::SetGoadEvent()
{
	auto VM = TryGetViewModel<UVM_CardDeck>(); check(VM);

	int32 Useless_Emblem = VM->GetUseless_EmblemType();

	if (VM->GetCurrentBossType() == EBossType::GOAD &&
		(Useless_Emblem == CardInfoData->Info.SuitGrade || CardInfoData->Info.EnforceType == EnforceStatType::WILD))
	{
		SetRenderOpacity(0.3f);
	}
	else
	{
		SetRenderOpacity(1.0f);
	}

}

void UCardButtonWidget::LoadEnhanceImage()
{
	int32 EnforceNum = static_cast<int32>(CardInfoData->Info.EnforceType);

	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/CardEnfoceImage/EnforceSprite_%d.EnforceSprite_%d"), EnforceNum, EnforceNum);
	CardInfoData->Info.EnforceSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!CardInfoData->Info.EnforceSprite.IsValid())
	{
		CardInfoData->Info.EnforceSprite.LoadSynchronous();
	}
}

void UCardButtonWidget::OnFilpAnimationFinished()
{
	MainImage->SetBrush(MainImageSpriteBrush);
	Enhance_Image->SetBrush(EnhanceImageSpriteBrush);
}
