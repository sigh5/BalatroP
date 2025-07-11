// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/ToolTip/ToolTipWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UToolTipWidget::SetWidgetPos(const FName& _Name, const FVector2D& Pos)
{
	if (Pos.X == 0 && Pos.Y == 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TestText->SetText(FText::FromName(_Name));

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
		if(CanvasSlot)
		{
			CanvasSlot->SetPosition(Pos);
		}
		SetVisibility(ESlateVisibility::Visible);
	}

}
