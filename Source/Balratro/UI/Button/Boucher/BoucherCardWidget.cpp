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

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Components/SizeBox.h"


UBoucherCardWidget::UBoucherCardWidget()
{

}

void UBoucherCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBoucherCardWidget::SetInfo()
{

	BuyButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UBoucherCardWidget::SetPriceText()
{

}

void UBoucherCardWidget::ChangeImage()
{

}

void UBoucherCardWidget::CreateImage()
{

}