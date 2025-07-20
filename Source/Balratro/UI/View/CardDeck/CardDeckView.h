// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "CardDeckView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UCardDeckView : public UBBUserWidgetBase
{
	GENERATED_BODY()
public:
	UCardDeckView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	
	void VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentDeckNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image0;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image5;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image6;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image7;*/

};
