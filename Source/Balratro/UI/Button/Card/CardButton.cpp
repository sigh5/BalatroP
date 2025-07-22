// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/Card/CardButton.h"

#include "Components/HorizontalBoxSlot.h"

void UCardButton::SetClikcedEvent()
{
	OnClicked.AddDynamic(this, &UCardButton::OnCardButtonClicked);
}

void UCardButton::OnCardButtonClicked()
{
	UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(Slot);
	check(HSlot);

	if (bSelected)
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top += 150.f;   // ���� �ø� ȿ�� (�Ʒ��� ���� ���̱�)
		HSlot->SetPadding(Margin);

		bSelected = false;
	}
	else
	{
		FMargin Margin = HSlot->GetPadding();
		Margin.Top -= 150.f;   // ���� �ø� ȿ�� (�Ʒ��� ���� ���̱�)
		HSlot->SetPadding(Margin);

		bSelected = true;
	}
}
