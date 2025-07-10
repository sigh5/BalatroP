// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRankingListView.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>

#include <Components/CanvasPanelSlot.h>
#include "UI/View/ToolTip/ToolTipWidget.h"
#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"

#include "Singleton\BBGameSingleton.h"

void UHandRankingListView::NativeConstruct()
{
	Super::NativeConstruct();

	InfoButton->OnHovered.AddDynamic(this, &UHandRankingListView::OnButtonHovered);
	InfoButton->OnUnhovered.AddDynamic(this, &UHandRankingListView::OnButtonUnhovered);

	InfoButton->ToolTipWidget = UBBGameSingleton::Get().MyToolTipWidget;
	
	auto Temp = UBBGameSingleton::Get().MyToolTipWidget;


	Temp->SetVisibility(ESlateVisibility::Hidden);
}

void UHandRankingListView::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (auto PartyMemberData = Cast<UHandRanking_Info>(ListItemObject))
	{
		MemberData = PartyMemberData;
	}

	UpdateVisuals();
}

void UHandRankingListView::OnButtonHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("Button: %s Pos: %s"),
		*GetNameSafe(this),
		*GetCachedGeometry().GetAbsolutePosition().ToString());

	auto Temp = UBBGameSingleton::Get().MyToolTipWidget;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ScreenCenter = ViewportSize * 0.5f;

	// 2. 툴팁 위젯의 크기 구하기
	FVector2D TooltipSize = Temp->GetDesiredSize();

	// 3. 중앙 정렬을 위한 위치 조정
	FVector2D AlignedPos = ScreenCenter - TooltipSize * 0.5f;

	FVector2D ButtonPos = GetCachedGeometry().GetAbsolutePosition();

	ScreenCenter.Y = ButtonPos.Y + 30.f;

	Temp->SetRenderTranslation(ScreenCenter);

	Temp->SetVisibility(ESlateVisibility::Visible);
}

void UHandRankingListView::OnButtonUnhovered()
{
	//InfoButton->ToolTipWidget->SetVisibility(ESlateVisibility::Hidden);
	auto Temp = UBBGameSingleton::Get().MyToolTipWidget;
	Temp->SetVisibility(ESlateVisibility::Hidden);
}

void UHandRankingListView::UpdateVisuals()
{
	if (!IsValid(MemberData))
	{
		return;
	}

	LevelText->SetText(FText::Format(FText::FromString("Lv.{0}"), FText::AsNumber(MemberData->Info.Level)));
	NameTextBlock->SetText(FText::FromName(MemberData->_Name));
	ChipText->SetText(FText::AsNumber(MemberData->Info.Chip));
	drainageText->SetText(FText::AsNumber(MemberData->Info.Drainage));
	NumTextBlock->SetText(FText::AsNumber(MemberData->Info.UseNum));
}
