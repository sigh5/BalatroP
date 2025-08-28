// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/WidgetPosMoveInterface.h"
#include "GameData/HandRankingToolTip_DataAsset.h"
#include "ToolTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UToolTipWidget : public UUserWidget , public IWidgetPosMoveInterface
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	virtual void SetHandRanking_ToolTipWidgetPos(const FHRButton_Info& _Info, const FVector2D& Pos) override;

	void		SetImageRender(class UImage* CurImage,class UPaperSprite* Sprite );

private:
	//void   Init
	
	void	Set_ToolTipWidgetImage(const FHRButton_Info& _Info);

private:
	UWidgetAnimation* GetAnimationByName(const FName& AnimName) const;
	void			FillAnimMap();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HaneRankingText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image5;

	UPROPERTY()
	TObjectPtr<UHandRankingToolTip_DataAsset> ToolTipDataAsset;


	TMap<FName, class UWidgetAnimation*> AnimationsMap;
};
