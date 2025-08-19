// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/Card/CardButton.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"

void UCardButton::SetCardInfoData(FDeckCardStat& InValue)
{
	CardInfoData = InValue;

	if (Image)
	{
		ChangeImage();
	}
	else
	{
		CreateImage();
	}
}

void UCardButton::SetClikcedEvent()
{
	bSelected = false;
	OnClicked.AddDynamic(this, &UCardButton::OnCardButtonClicked);
}

void UCardButton::OnCardButtonClicked()
{
	auto VM = GetVMCardDeck();
	bool CurSelectedMax = VM->GetIsSelectedMax();

	if (CurSelectedMax == true && bSelected == false) // 5장이 꽉찼고 추가적으로 더 선택할 경우 return;
	{
		return;
	}

	bSelected = !bSelected; // Toggle
	
	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	check(HSlot);
	if (bSelected == false )
	{
		FMargin Margin = HSlot->GetPadding(); 
		Margin.Top += 150.f;  // 선택한 카드 내리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
	else if(bSelected == true && CurSelectedMax == false)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 150.f;    // 선택한 카드 올리는 것
		HSlot->SetPadding(Margin);

		VM->SetIsUpCardExist(true);
	}
}

UVM_CardDeck* UCardButton::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}

void UCardButton::ChangeImage()
{
	if (UPaperSprite* Sprite = CardInfoData.CardSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(100.f, 150.f);
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		Image->SetBrush(SpriteBrush);

		Border->SetContent(Image);
	}
}

void UCardButton::CreateImage()
{
	USizeBox* SizeBox = NewObject<USizeBox>(this);
	SizeBox->SetWidthOverride(100.f);
	SizeBox->SetHeightOverride(150.f);
	SetContent(SizeBox);

	Border = NewObject<UBorder>(SizeBox);
	Border->SetBrushColor(FLinearColor::White);
	Border->SetHorizontalAlignment(HAlign_Fill);
	Border->SetVerticalAlignment(VAlign_Fill);
	SizeBox->SetContent(Border);

	if (UPaperSprite* Sprite = CardInfoData.CardSprite.Get())
	{
		Image = NewObject<UImage>(Border);
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(100.f, 150.f);
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		Image->SetBrush(SpriteBrush);
		Border->SetContent(Image);

		FButtonStyle EmptyStyle;
		EmptyStyle.SetNormal(FSlateNoResource());
		EmptyStyle.SetHovered(FSlateNoResource());
		EmptyStyle.SetPressed(FSlateNoResource());
		SetStyle(EmptyStyle);
		SetClikcedEvent();
	}
}
