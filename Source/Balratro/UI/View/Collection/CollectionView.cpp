// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/Collection/CollectionView.h"
#include "UI/MVVM/ViewModel/VM_Collection.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"

#include "PaperSprite.h"

#include "GameData/BlindStat.h"

#include "UI/Button/JokerCard/JokerCardWidget.h"

UCollectionView::UCollectionView()
{
	ViewModelClass = UVM_Collection::StaticClass();
	ViewModelName = TEXT("VM_Collection");
}

void UCollectionView::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCollectionView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	for (UWidget* Child : JokerWrapBox->GetAllChildren())
	{
		if (UJokerCardWidget* Card = Cast<UJokerCardWidget>(Child))
		{
			JokerCards.Add(Card);
		}
	}

	const auto VMInst = TryGetViewModel<UVM_Collection>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	/*VMInst->AddFieldValueChangedDelegate(UVM_Collection::FFieldNotificationClassDescriptor::JokerCollection,
		FFieldValueChangedDelegate::CreateUObject(this, &UCollectionView::VM_FieldChanged_JokerCollection));*/
}

void UCollectionView::VM_FieldChanged_JokerCollection(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Collection>(); check(VMInst);



}
