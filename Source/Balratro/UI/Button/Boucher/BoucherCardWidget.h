// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "BoucherCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UBoucherCardWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UBoucherCardWidget();

private:
	virtual void NativeConstruct() override;

public:
	void	SetInfo();
	void	SetPriceText();

private:
	void	ChangeImage();
	void	CreateImage();



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

};
