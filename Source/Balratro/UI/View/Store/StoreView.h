// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "StoreView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UStoreView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UStoreView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnNextButton();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> NextButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReRollButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ReRollText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> UpHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> BoucherHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> PackHorizontalBox;

};
