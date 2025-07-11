// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRanking/HandRankingListView.h"

#include <Components/TextBlock.h>
#include <Components/Button.h>

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include <Blueprint/WidgetLayoutLibrary.h>


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

	FVector2D ButtonScreenPos = InfoButton->GetCachedGeometry().GetAbsolutePosition();

	ButtonScreenPos.Y += 50.f;

	FHRButton_Info Info;
	Info.Pos = ButtonScreenPos;
	Info._Name = MemberData->_Name;

	VM->SetHRButtonInfo(Info);
}

void UHandRankingListView::OnButtonUnhovered()
{
	auto VM = GetVMHandRanking();
	
	FHRButton_Info Info;
	Info.Pos = FVector2D::Zero();
	Info._Name = MemberData->_Name;
	
	VM->SetHRButtonInfo(Info);
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
