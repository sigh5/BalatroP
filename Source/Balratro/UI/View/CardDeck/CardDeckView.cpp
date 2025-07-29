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


#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
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

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurrentAllHands,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_HandInCard));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::IsUpCardExist,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CardUpExist));
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
	HandCardButton.Empty();
	// ���߿� ������Ʈ Ǯ ���� �ذ��ϱ�
	const auto VMInstance = Cast<UVM_CardDeck>(Object);
	auto& CurHandInfo = VMInstance->GetCurrentAllHands();
	int32 CurHandNum = CurHandInfo.Num();
	const int32 PaddingX = 8;

	CardPanel->ClearChildren(); // ���� �̹��� ����
	
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

		// ��ư �⺻ ��� ����(����)
		FButtonStyle EmptyStyle;
		EmptyStyle.SetNormal(FSlateNoResource());
		EmptyStyle.SetHovered(FSlateNoResource());
		EmptyStyle.SetPressed(FSlateNoResource());
		NewButton->SetStyle(EmptyStyle);
		
		NewButton->SetSelected(false);
		NewButton->SetClikcedEvent();

		HandCardButton.Add(NewButton);
	}

	bool c = false;
}

void UCardDeckView::VM_FieldChanged_CardUpExist(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	TArray<FDeckCardStat> CardStatInfo;
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
	{
		if (SelectedNum == 0)
		{
			VMInst->BrodCastrHandRankName(0, CardStatInfo);
		}
	}

	VMInst->BrodCastrHandRankName(SelectedNum, CardStatInfo);
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
	
	TArray<FDeckCardStat> CardStatInfo; 
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
		return;
	
	VMInst->UseChuck(SelectedNum, CardStatInfo);
}

void UCardDeckView::OnHandPlayButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();

	TArray<FDeckCardStat> CardStatInfo;
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
		return;

	VMInst->UseHandPlay(SelectedNum, CardStatInfo);
}

bool UCardDeckView::SetCardData(OUT TArray<FDeckCardStat>& CardStatInfo, OUT int32& SelectedCardNum)
{
	for (auto& Button : HandCardButton)
	{
		if (Button->GetSelected())
		{
			++SelectedCardNum;
			CardStatInfo.Add(Button->GetCardInfoData());
		}
	}
	
	if (SelectedCardNum == 0)
		return false;

	return true;
}
