// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/DeckCardStat.h"
#include "CardButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UCardButtonWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UCardButtonWidget();

private:
	virtual void NativeConstruct()override;

public:
	FORCEINLINE void		SetCardIndex(int InValue) { CardIndex = InValue; }

	void					SetInfo(UHandInCard_Info* _inValue);
	FORCEINLINE				UHandInCard_Info* GetCardInfoData() { return CardInfoData; }

	FORCEINLINE const bool	GetSelected() const { return bSelected; }
	FORCEINLINE void		SetSelected(bool _Selected) { bSelected = _Selected; }

private:
	UFUNCTION()
	void OnCardButtonClicked();

	void LoadEnhanceImage();

private:
	void	SetClikcedEvent();

	void	ChangeImage();
	void	CreateImage();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Overlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> ButtonOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MainImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Enhance_Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Ghost_Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Foil_Image;

private:
	int CardIndex = 0;
	
	UPROPERTY()
	class UHandInCard_Info* CardInfoData;

	uint8 bSelected : 1;
	uint8 IsCreated : 1;
};
