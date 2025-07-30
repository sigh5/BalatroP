// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "BlindSelectView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UBlindSelectView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UBlindSelectView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnSmallBlindButtonClicked();

private:
	void VM_FieldChanged_SmallBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_BigBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_BossBlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SmallBlindButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SmallButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SmallBlindScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SmallBlind_RewardText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SmallSkipButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BigBlindButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BigButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BigBlindScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BigBlind_RewardText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BiglSkipButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BosslBlindButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BosslButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BossNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BossBlindImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BossBlindScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BossBlind_RewardText;

};
