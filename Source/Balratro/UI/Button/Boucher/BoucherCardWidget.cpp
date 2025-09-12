#include "UI/Button/Boucher/BoucherCardWidget.h"

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
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"


UBoucherCardWidget::UBoucherCardWidget()
	: IsStoreHave(false),IsCreated(false), IsSelcte(false)
{
	ViewModelClass = UVM_Store::StaticClass();
	ViewModelName = "VM_Store";
}

void UBoucherCardWidget::SetClikcedEvent()
{
	IsSelcte = false;
	MainButton->OnClicked.AddDynamic(this, &UBoucherCardWidget::OnClickedEvent);
	BuyButton->OnClicked.AddDynamic(this, &UBoucherCardWidget::OnBuyClickedEvent);
}

void UBoucherCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBoucherCardWidget::SetInfo(FBoucherInfo& Data)
{
	BuyButton->SetVisibility(ESlateVisibility::Collapsed);

	BoucherInfo = Data;

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
}

void UBoucherCardWidget::SetIsStoreHave(bool _InValue)
{
	IsStoreHave = _InValue;

	if (IsStoreHave)
	{
		FString Pricestr = FString::Printf(TEXT("$%d"), BoucherInfo.Price);
		PriceText->SetText(FText::FromString(Pricestr));
		PriceText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PriceText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
}

void UBoucherCardWidget::ChangeImage()
{
	if (UPaperSprite* Sprite = BoucherInfo.CardSprite.Get())
	{
		FSlateBrush SpriteBrush;
		SpriteBrush.SetResourceObject(Sprite);
		SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
		SpriteBrush.SetImageSize(FVector2D(100.f, 150.f));
		MainImage->SetBrush(SpriteBrush);
	}
}

void UBoucherCardWidget::CreateImage()
{
	if (!BoucherInfo.CardSprite.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("BoucherInfo.CardSprite.IsValid()"));
		BoucherInfo.CardSprite.LoadSynchronous();
	}


	FButtonStyle EmptyStyle;
	EmptyStyle.SetNormal(FSlateNoResource());
	//EmptyStyle.SetHovered(FSlateNoResource());
	EmptyStyle.SetPressed(FSlateNoResource());
	MainButton->SetStyle(EmptyStyle);

	ChangeImage();
	SetClikcedEvent();
}

void UBoucherCardWidget::OnClickedEvent()
{
	IsSelcte = !IsSelcte;

	if (IsStoreHave)
	{
		UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
		FMargin Margin = HSlot->GetPadding();

		if (IsSelcte)
		{
			Margin.Top -= 50.f;  // 선택한 카드 내리는 것
			BuyButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Margin.Top += 50.f;  // 선택한 카드 올리는 것
			BuyButton->SetVisibility(ESlateVisibility::Collapsed);
		}

		HSlot->SetPadding(Margin);

		UE_LOG(LogTemp, Warning, TEXT("OnBuyClickedEvent"));
	}
}

void UBoucherCardWidget::OnBuyClickedEvent()
{
	auto VM = TryGetViewModel<UVM_Store>(); check(VM);

	const auto VMPlayerInfo = TryGetViewModel<UVM_PlayerInfo>("VM_PlayerInfo", UVM_PlayerInfo::StaticClass()); check(VMPlayerInfo);
	int32 CurGold = VMPlayerInfo->GetGold();

	if (CurGold - BoucherInfo.Price < 0)
	{
		ShakingAnimStart();
		return;
	}

	VM->BuyBoucherCard(BoucherInfo);

	SetVisibility(ESlateVisibility::Collapsed);
	IsStoreHave = false;
}

void UBoucherCardWidget::OnButtonHoverEvent()
{
	// ToDO 툴팁 제작

}


void UBoucherCardWidget::ShakingAnimStart()
{
	//PlayAnimation(ShakingAnim);
}
