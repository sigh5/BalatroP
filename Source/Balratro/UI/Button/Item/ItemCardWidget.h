// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/TaroStat.h"
#include "ItemCardWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UItemCardWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UItemCardWidget();

private:
	virtual void NativeConstruct()override;

public:
	void	SetInfo(FTaroStat& _inValue);
	void	SetCreatePlayerInfoView(bool _InValue) { IsCreatePlayerInfoView = _InValue; }


private:
	UFUNCTION()
	void					OnItemButtonClicked();

	UFUNCTION()
	void					OnItemUseClicked();

	void					SetClikcedEvent();

private:
	void	ChangeImage();
	void	CreateImage();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Overlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> UseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MainImage;

private:
	FTaroStat TaroData;

	uint8 IsSelected : 1;

	uint8 IsCreated : 1;

	uint8 IsCreatePlayerInfoView : 1;
};
