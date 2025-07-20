// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/CardDeck/CardDeckView.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "Styling/SlateBrush.h"

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
}

void UCardDeckView::VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_CardDeck>(Object);

	CurrentDeckNumText->SetText(FText::AsNumber(VMInstance->GetDeckNum()));
}

void UCardDeckView::VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_CardDeck>(Object);

	// /Script/Paper2D.PaperSprite'/Game/CardResuorce/Card/C_2.C_2'
	auto CurHandInfo = 	VMInstance->GetCurrentHandInCards();
	int32 CurHandNum = CurHandInfo.Num();


	//for (int i = 0; i < CurHandNum; ++i)
	{
		FName CurName= CurHandInfo[0]->Info.Name;
		FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Card/%s.%s"), *CurName.ToString(), *CurName.ToString());

		UPaperSprite* Sprite = LoadObject<UPaperSprite>(nullptr, *AssetPath);
		if (Sprite)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(Sprite);

			Image0->SetBrush(Brush);
		}

	
	}


}
