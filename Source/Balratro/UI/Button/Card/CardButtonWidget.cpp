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
}


void UCardButtonWidget::SetClikcedEvent()
{
	bSelected = false;
	MainButton->OnClicked.AddDynamic(this, &UCardButtonWidget::OnCardButtonClicked);
}


void UCardButtonWidget::OnCardButtonClicked()
{
	const auto VM = TryGetViewModel<UVM_CardDeck>();
	check(VM);

	bool CurSelectedMax = VM->GetIsSelectedMax();

	if (CurSelectedMax == true && bSelected == false) // 5장이 꽉찼고 추가적으로 더 선택할 경우 return;
	{
		return;
	}

	bSelected = !bSelected; // Toggle

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

void UCardButtonWidget::SetInfo(FDeckCardStat& _inValue)
{
	CardInfoData = _inValue;

	if (IsCreated == false)
		CreateImage();
	else
		ChangeImage();


	if (IsCreated == false)
		IsCreated = true;
}


void UCardButtonWidget::ChangeImage()
{
	if (UPaperSprite* Sprite = CardInfoData.CardSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		SpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		MainImage->SetBrush(SpriteBrush);
	}

	if (UPaperSprite* Sprite = CardInfoData.EnforceSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		SpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		Enhance_Image->SetBrush(SpriteBrush);
	}

}

void UCardButtonWidget::CreateImage()
{
	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	//EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);

	ChangeImage();
	SetClikcedEvent();
}