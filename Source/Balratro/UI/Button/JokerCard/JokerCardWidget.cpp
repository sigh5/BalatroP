#include "UI/Button/JokerCard/JokerCardWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "UI/MVVM/ViewModel/VM_ItemSelect.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"


#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"


UJokerCardWidget::UJokerCardWidget()
{
	ViewModelClass = UVM_JockerSlot::StaticClass();
	ViewModelName = TEXT("VM_JockerSlot");
}

void UJokerCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJokerCardWidget::SetInit()
{
	IsSelected = false;
	MainButton->OnClicked.AddDynamic(this, &UJokerCardWidget::OnJokerButtonClicked);
	EventButton->OnClicked.AddDynamic(this, &UJokerCardWidget::OnSellButtonClicked);
}

void UJokerCardWidget::SetInfo(FJokerStat& _inValue)
{
	JokerData = _inValue;

	if (IsCreated == false)
		CreateJokerImage();
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

void UJokerCardWidget::OnSellButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_JockerSlot>(); check(VMInst);

	IsStore = !IsStore;
	SetVisibility(ESlateVisibility::Collapsed);
	
	VMInst->SetAddJokerCard(JokerData);
}

void UJokerCardWidget::OnButtonHover()
{

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
}

void UJokerCardWidget::ChangeJokerImage()
{
	if (UPaperSprite* Sprite = JokerData.CardSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(126.f, 186.f); // 원하는 크기
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImage->SetBrush(SpriteBrush);
	}

	FString Pricestr = FString::Printf(TEXT("$%d"), JokerData.Price);
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


