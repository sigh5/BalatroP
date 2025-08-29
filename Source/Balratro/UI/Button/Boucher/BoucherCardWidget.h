// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/BoucherStat.h"
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
	void	SetInfo(FBoucherInfo& Data);

	void	SetIsStoreHave(bool _InValue);


private:
	void	SetClikcedEvent();
	
	void	ChangeImage();
	void	CreateImage();

	UFUNCTION()
	void	OnClickedEvent();

	UFUNCTION()
	void	OnButtonHoverEvent();

	UFUNCTION()
	void	OnBuyClickedEvent();

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

private:
	uint8 IsStoreHave : 1;
	uint8 IsCreated : 1;
	uint8 IsSelcte : 1;


	FBoucherInfo BoucherInfo;
};
