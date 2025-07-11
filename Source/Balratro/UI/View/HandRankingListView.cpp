// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRankingListView.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"


void UHandRankingListView::NativeConstruct()
{
	Super::NativeConstruct();

	InfoButton->OnHovered.AddDynamic(this, &UHandRankingListView::OnButtonHovered);
	InfoButton->OnUnhovered.AddDynamic(this, &UHandRankingListView::OnButtonUnhovered);
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
	auto VM = GetVMHandRanking();

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ScreenCenter = ViewportSize * 0.5f;
	
	FVector2D ButtonPos = GetCachedGeometry().GetAbsolutePosition();
	ScreenCenter.Y = ButtonPos.Y + 30.f;

	VM->SetWidgetPos(ScreenCenter);
}

void UHandRankingListView::OnButtonUnhovered()
{
	FVector2D ZeroPos = FVector2D::Zero();

	auto VM = GetVMHandRanking();
	VM->SetWidgetPos(ZeroPos);
}

UVM_HandRankingCount* UHandRankingListView::GetVMHandRanking()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_HandRankingCount");
	Context.ContextClass = UVM_HandRankingCount::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_HandRankingCount>(Found);
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
