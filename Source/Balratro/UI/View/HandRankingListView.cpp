// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/HandRankingListView.h"

#include <Components/TextBlock.h>
#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"

void UHandRankingListView::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (auto PartyMemberData = Cast<UHandRanking_Info>(ListItemObject))
	{
		MemberData = PartyMemberData;
	}

	UpdateVisuals();
}

void UHandRankingListView::UpdateVisuals()
{
	if (!IsValid(MemberData))
	{
		return;
	}

	NameTextBlock->SetText(MemberData->Name);
	NumTextBlock->SetText(FText::AsNumber(MemberData->UseNum));
}
