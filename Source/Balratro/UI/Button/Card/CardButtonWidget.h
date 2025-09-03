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

	FWidgetAnimationDynamicEvent EndDelegate;

private:
	virtual void NativeConstruct()override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool   NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void	NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	
public:
	FORCEINLINE void		SetCardIndex(int InValue) { CardIndex = InValue; }

	void					SetInfo(UHandInCard_Info* _inValue);
	FORCEINLINE				UHandInCard_Info* GetCardInfoData() { return CardInfoData; }

	FORCEINLINE const bool	GetSelected() const { return bSelected; }
	FORCEINLINE void		SetSelected(bool _Selected) { bSelected = _Selected; }

	void					MoveAnimmation();
	void					ShakeAnimation();

	void					DrawAnimation();
	void					ChuckAnimation();
private:
	UFUNCTION()
	void OnFilpAnimationFinished();

private:
	void	OnCardButtonClicked();
	void	LoadEnhanceImage();
	void	ChangeImage();
	void	CreateImage();

	void	 ForceSwapData(class UHandInCard_Info* CardInfoData0);
	void	 ForceSwapPos();


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

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> MoveAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> MoveEndAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FilpAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ShakeAnim;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ChunkAnim;


private:
	int CardIndex = 0;
	
	FSlateBrush MainImageSpriteBrush;
	FSlateBrush EnhanceImageSpriteBrush;

	uint8 bSelected : 1;
	uint8 IsCreated : 1;
	uint8 bIsDragging : 1;
	uint8 bIsDummyData : 1;

	FVector2D DragOffset;

	UPROPERTY()
	TObjectPtr<class UHandInCard_Info> CardInfoData;

	UPROPERTY()
	TObjectPtr<class UCardButtonWidget> DragVisual;

};
