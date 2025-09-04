// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameOverView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UGameOverView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UGameOverView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	void VM_FieldChanged_GameOver_INT_Data(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_MostHandRankingName(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_SeedName(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SeedCopyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> NewRunButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BestHandText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MostHandRankingText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ChuckNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ReRollNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SeedText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> EntiNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RoundNumText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DefeatImage;

};
