// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "Item/BoosterPackData.h"
#include "BoosterPackWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UBoosterPackWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UBoosterPackWidget();

private:
	virtual void NativeConstruct()override;

public:
	void	SetInfo(UBoosterPackData* _inValue);

	void	SetPriceText();

private:
	void	ChangeImage();
	void	CreateImage();

private:
	UFUNCTION()
	void					OnBoosterPackButtonClicked();

	UFUNCTION()
	void					OnBuyButtonClicked();

	void					SetClikcedEvent();


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Overlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MainImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BuyButton;


	UPROPERTY()
	TObjectPtr<UBoosterPackData> PackData;

	uint8 IsSelected : 1;

	uint8 IsCreated : 1;


};
