// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/JokerStat.h"
#include "JokerCardWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UJokerCardWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UJokerCardWidget();

private:
	virtual void NativeConstruct()override;
	virtual void NativeOnInitialized()override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool   NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void	NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;


public:
	void	SetInfo(UJokerCard_Info* _inValue);
	const UJokerCard_Info* GetInfo() const { return JokerData; }

	void	 ShakingEvent();
	void	 PlayJokerEvent(class UTextBlock* SkillText0, class UTextBlock* SkillText1);

	void	SetCopyJoker(EJokerType JokerType);


	FORCEINLINE const bool	GetSelected() const { return IsSelected; }
	FORCEINLINE void		SetSelected(bool _Selected) { IsSelected = _Selected; }

	FORCEINLINE const bool	GetIsStoreHave() const { return IsStore; }
	void		SetIsStoreHave(bool _Selected);

private:
	UFUNCTION()
	void					OnJokerButtonClicked();

	UFUNCTION()
	void					OnSellButtonClicked();

	UFUNCTION()
	void					OnButtonHover();


	void ForceSwapData(UJokerCard_Info* InfoData);

private:
	void					SetInit();
	void					ChangeJokerImage();
	void					CreateJokerImage();

	void					ForceSwapPos();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Overlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> BuyBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BuyPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> EventButton; // Sell Buy

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MainImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> ShakingAnim;

private:
	TObjectPtr<class UJokerCard_Info>	JokerData;

	uint8 IsSelected : 1;
	uint8 IsCreated : 1;
	uint8 IsStore : 1;  // true 상점거 false 조커뷰
	uint8 bIsDragging : 1;
	uint8 bIsDummyData : 1;

	FVector2D DragOffset;

	UPROPERTY()
	TObjectPtr<class UJokerCardWidget> DragVisual;

};
