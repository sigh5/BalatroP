// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BoosterPack/BoosterPackWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "UI/MVVM/ViewModel/VM_Store.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"

UBoosterPackWidget::UBoosterPackWidget()
{
	ViewModelName = TEXT("VM_Store");
	ViewModelClass = UVM_Store::StaticClass();
}

void UBoosterPackWidget::SetClikcedEvent()
{
	IsSelected = false;
	MainButton->OnClicked.AddDynamic(this, &UBoosterPackWidget::OnBoosterPackButtonClicked);
	BuyButton->OnClicked.AddDynamic(this, &UBoosterPackWidget::OnBuyButtonClicked);
}

void UBoosterPackWidget::OnBuyButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_Store>();
	check(VMInst);

	VMInst->OnClickedBuyBoosterPackButton(PackData);

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UBoosterPackWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBoosterPackWidget::SetInfo(UBoosterPackData* _inValue)
{
	PackData = _inValue;

	BuyButton->SetVisibility(ESlateVisibility::Collapsed);

	if (IsCreated == false)
		CreateImage();
	else
		ChangeImage();

	SetPriceText();

	if (IsCreated == false)
		IsCreated = true;

}

void UBoosterPackWidget::SetPriceText()
{
	auto Type = PackData->GetType();
	int32 TypeIndex = static_cast<int32>(Type);
	
	if (TypeIndex % 2 == 1) // 홀수 mega = 6
	{
		PriceText->SetText(FText::FromString(TEXT("$6")));
	}
	else    // 짝수 기본 4
	{
		PriceText->SetText(FText::FromString(TEXT("$4")));
	}

}

void UBoosterPackWidget::ChangeImage()
{
	if (UPaperSprite* Sprite = PackData->PackMesh.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(100.f, 150.f); // 원하는 크기
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImage->SetBrush(SpriteBrush);
	}

}

void UBoosterPackWidget::CreateImage()
{
	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);

	ChangeImage();
	SetClikcedEvent();
}

void UBoosterPackWidget::OnBoosterPackButtonClicked()
{
	IsSelected = !IsSelected;

	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	FMargin Margin = HSlot->GetPadding();

	if (IsSelected)
	{	
		Margin.Top -= 100.f;  // 선택한 카드 내리는 것
		BuyButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Margin.Top += 100.f;  // 선택한 카드 올리는 것
		BuyButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	HSlot->SetPadding(Margin);
}
