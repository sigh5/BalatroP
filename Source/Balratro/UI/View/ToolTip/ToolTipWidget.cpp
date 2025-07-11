// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/ToolTip/ToolTipWidget.h"

void UToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UToolTipWidget::SetWidgetPos(FVector2D& Pos)
{
	if (Pos.X == 0 && Pos.Y == 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		FVector2D TooltipSize = GetDesiredSize();
		FVector2D AlignedPos = Pos - TooltipSize * 0.5f;

		AlignedPos.Y = Pos.Y;

		SetRenderTranslation(AlignedPos);
		SetVisibility(ESlateVisibility::Visible);
	}

}
