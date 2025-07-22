// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/CardDeck/CardDeckView.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"

#include "UI/Button/Card/CardButton.h"


#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"

#include "GameData/HandRankingStat.h"

#include "UI/MVVM/ViewModel/VM_CardDeck.h"


UCardDeckView::UCardDeckView()
{
	ViewModelClass = UVM_CardDeck::StaticClass();
	ViewModelName = TEXT("VM_CardDeck");
}

void UCardDeckView::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::DeckNum,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_DeckNum));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurrentHandInCards,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_HandInCard));

}

void UCardDeckView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SuitSortButton->OnClicked.AddDynamic(this, &UCardDeckView::OnSuitSortButtonClicked);
	RankSortButton->OnClicked.AddDynamic(this, &UCardDeckView::OnRankSortButtonClicked);
	HandPlayButton->OnClicked.AddDynamic(this, &UCardDeckView::OnHandPlayButtonClicked);
	ChuckButton->OnClicked.AddDynamic(this, &UCardDeckView::OnChuckButtonClicked);
}

void UCardDeckView::VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_CardDeck>(Object);

	CurrentDeckNumText->SetText(FText::AsNumber(VMInstance->GetDeckNum()));
}

void UCardDeckView::VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	// 나중에 오브젝트 풀 만들어서 해결하기
	const auto VMInstance = Cast<UVM_CardDeck>(Object);
	auto& CurHandInfo = VMInstance->GetCurrentHandInCards();
	int32 CurHandNum = CurHandInfo.Num();
	const int32 PaddingX = 8;

	CardPanel->ClearChildren(); // 기존 이미지 제거
	for (int i = 0; i < CurHandNum; ++i)
	{
		UCardButton* NewButton = NewObject<UCardButton>(this);
		NewButton->SetCardInfoData(CurHandInfo[i]->Info);
		NewButton->SetCardIndex(CardIndex++);

		USizeBox* SizeBox = NewObject<USizeBox>(NewButton);
		SizeBox->SetWidthOverride(100.f);
		SizeBox->SetHeightOverride(150.f);
		NewButton->SetContent(SizeBox);

		UBorder* BackgroundBorder = NewObject<UBorder>(SizeBox);
		BackgroundBorder->SetBrushColor(FLinearColor::White);
		BackgroundBorder->SetHorizontalAlignment(HAlign_Fill);
		BackgroundBorder->SetVerticalAlignment(VAlign_Fill);
		SizeBox->SetContent(BackgroundBorder);

		if (UPaperSprite* Sprite = CurHandInfo[i]->Info.CardSprite.Get())
		{
			UImage* CardImage = NewObject<UImage>(BackgroundBorder);
			FSlateBrush SpriteBrush;
			SpriteBrush.SetResourceObject(Sprite);
			SpriteBrush.ImageSize = FVector2D(100.f, 150.f);
			SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
			CardImage->SetBrush(SpriteBrush);

			BackgroundBorder->SetContent(CardImage);
		}

		UHorizontalBoxSlot* BoxSlot = CardPanel->AddChildToHorizontalBox(NewButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));

		FMargin Padding0;
		if (i == 0)
			Padding0 = FMargin(PaddingX * 2, 0.f, PaddingX, 0.f);
		//else if (i == CurHandNum - 1)
			//Padding0 = FMargin(PaddingX, 0.f, PaddingX, 0.f);
		else
			Padding0 = FMargin(PaddingX, 0.f, PaddingX, 0.f);
		BoxSlot->SetPadding(Padding0);

		BoxSlot->SetHorizontalAlignment(HAlign_Center);
		BoxSlot->SetVerticalAlignment(VAlign_Center);

		// 버튼 기본 배경 제거(투명)
		FButtonStyle EmptyStyle;
		EmptyStyle.SetNormal(FSlateNoResource());
		EmptyStyle.SetHovered(FSlateNoResource());
		EmptyStyle.SetPressed(FSlateNoResource());
		NewButton->SetStyle(EmptyStyle);
		
		NewButton->SetClikcedEvent();

		HandCardButton.Add(NewButton);
	}

}

void UCardDeckView::OnSuitSortButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	VMInst->SetSuitSort();
}

void UCardDeckView::OnRankSortButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	VMInst->SetRankSort();
}

void UCardDeckView::OnChuckButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	
	int Selected = 0;

	TArray<FDeckCardStat> CardStatInfo;

	for (auto& Button : HandCardButton)
	{
		if (Button->GetSelected())
		{
			++Selected;

			Button->SetSelected(false);
			CardStatInfo.Add(Button->GetCardInfoData());
		}
	}
	
	VMInst->UseChuck(Selected, CardStatInfo);
}

void UCardDeckView::OnHandPlayButtonClicked()
{

}
