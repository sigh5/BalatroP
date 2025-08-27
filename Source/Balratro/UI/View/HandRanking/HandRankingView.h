// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "HandRankingView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UHandRankingView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UHandRankingView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:

	void VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_WidgetPos(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	UFUNCTION()
	void  OnClickedExitButton();

	UFUNCTION()
	void  OnClicked_PokerHandButton();

	UFUNCTION()
	void  OnClicked_BlindButton();


	UFUNCTION()
	void  OnClicked_VoucherButton();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> MyCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> HandRankingListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUserWidget> HandRankingToolTipWidget;


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> PokerHandButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BlindButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> VoucherButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

};
