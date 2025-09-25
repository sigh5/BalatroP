// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "CollectionView.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UCollectionView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UCollectionView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	void VM_FieldChanged_JokerCollection(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BackButton;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> JokerWrapBox;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Joker;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Boucher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Taro;

	UPROPERTY()
	TArray<class UJokerCardWidget*> JokerCards;

};
