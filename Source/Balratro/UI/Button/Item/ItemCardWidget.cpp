// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCardWidget.h"

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
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"


UItemCardWidget::UItemCardWidget()
{
	ViewModelName = TEXT("VM_ItemSelect");
	ViewModelClass = UVM_ItemSelect::StaticClass();
}

void UItemCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UItemCardWidget::SetClikcedEvent()
{
	IsSelected = false;
	MainButton->OnClicked.AddDynamic(this, &UItemCardWidget::OnItemButtonClicked);
	UseButton->OnClicked.AddDynamic(this, &UItemCardWidget::OnItemUseClicked);
}

void UItemCardWidget::SetInfo(FTaroStat& _inValue)
{
	TaroData = _inValue;

	if (IsCreated == false)
		CreateImage();
	else
		ChangeImage();

	IsSelected = false;
	SetVisibility(ESlateVisibility::Visible); // ��� ��ư ���̰� �ʱ�ȭ

	UseButton->SetVisibility(ESlateVisibility::Collapsed);

	if (IsCreated == false)
		IsCreated = true;
}

void UItemCardWidget::OnItemButtonClicked()
{
	IsSelected = !IsSelected;

	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	FMargin Margin = HSlot->GetPadding();

	if (IsSelected)
	{
		Margin.Top -= 100.f;  // ������ ī�� ������ ��
		UseButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Margin.Top += 100.f;  // ������ ī�� �ø��� ��
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	HSlot->SetPadding(Margin);
}

void UItemCardWidget::OnItemUseClicked()
{
	const auto VM_Inst = TryGetViewModel<UVM_ItemSelect>();
	check(VM_Inst);

	auto CurCards = VM_Inst->GetSelectCardInfos();

	UE_LOG(LogTemp, Warning, TEXT("TaroData.maxUseCardNum : %d"), TaroData.maxUseCardNum);

	if (TaroData.maxUseCardNum == 0)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		VM_Inst->UseTaroItem(TaroData);
		// 0�� �ֵ��� �׳� ��밡����
	}
	else if (TaroData.maxUseCardNum > 0 && TaroData.maxUseCardNum < CurCards.Num())
	{
		// 0�� �ƴϰ� ���� ������ �Ȱɸ�
		VM_Inst->UseTaroItem(TaroData);
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		// ȭ�� ��鸮�� �ִϸ��̼� �ʿ�
		return;
	}
}

void UItemCardWidget::ChangeImage()
{
	if (UPaperSprite* Sprite = TaroData.SpriteAsset.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.ImageSize = FVector2D(126.f, 186.f); // ���ϴ� ũ��
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		MainImage->SetBrush(SpriteBrush);
	}

}

void UItemCardWidget::CreateImage()
{
	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	//EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);

	ChangeImage();
	SetClikcedEvent();
}
