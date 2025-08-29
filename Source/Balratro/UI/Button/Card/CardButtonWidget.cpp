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
	EndDelegate.BindDynamic(this, &UCardButtonWidget::OnFilpAnimationFinished); // C++ 함수
	BindToAnimationFinished(FilpAnim, EndDelegate);

}

FReply UCardButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown"));
		// 드래그 가능 상태이면 감지 시작
		return Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown == false"));

	return Reply;
}

void UCardButtonWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected"));

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
	DragOp->DefaultDragVisual = MainImage;        // 드래그 중 보여줄 비주얼
	DragOp->Pivot = EDragPivot::MouseDown;   // 마우스 기준 드래그

	OutOperation = DragOp;
	
}

bool UCardButtonWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (InOperation && InOperation->Payload)
	{
		// 예: 드래그한 위젯 가져오기

		UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop"));
		return true;
	}

	return false;
}

void UCardButtonWidget::SetClikcedEvent()
{
	bSelected = false;
	MainButton->OnClicked.AddDynamic(this, &UCardButtonWidget::OnCardButtonClicked);
}

void UCardButtonWidget::MoveAnimmation()
{
	PlayAnimation(MoveAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCardButtonWidget::DrawAnimation()
{
	PlayAnimation(MoveEndAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
}

void UCardButtonWidget::OnCardButtonClicked()
{
	const auto VM = TryGetViewModel<UVM_CardDeck>();
	check(VM);

	bool CurSelectedMax = VM->GetIsSelectedMax();

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

	if (IsCreated == false)
	{
		CreateImage();
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
		
	}

	if (UPaperSprite* Sprite = CardInfoData->Info.EnforceSprite.Get())
	{
		EnhanceImageSpriteBrush.SetResourceObject(Sprite);
		EnhanceImageSpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		EnhanceImageSpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
	
	}

}

void UCardButtonWidget::CreateImage()
{
	//FButtonStyle EmptyStyle;
	//EmptyStyle.SetNormal(FSlateNoResource());
	////EmptyStyle.SetHovered(FSlateNoResource());
	//EmptyStyle.SetPressed(FSlateNoResource());
	//MainButton->SetStyle(EmptyStyle);

	ChangeImage();
	SetClikcedEvent();
}

void UCardButtonWidget::LoadEnhanceImage()
{
	int32 EnforceNum = static_cast<int32>(CardInfoData->Info.EnforceType);

	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/CardEnfoceImage/EnforceSprite_%d.EnforceSprite_%d"),EnforceNum, EnforceNum);
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
