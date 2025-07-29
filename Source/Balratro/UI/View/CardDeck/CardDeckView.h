// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "CardDeckView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UCardDeckView : public UBBUserWidgetBase
{
	GENERATED_BODY()
public:
	UCardDeckView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;


	void VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_CardUpExist(UObject* Object, UE::FieldNotification::FFieldId FieldId);


	/*UFUNCTION()
	void OnCardButtonClicked();*/

	UFUNCTION()
	void OnSuitSortButtonClicked();

	UFUNCTION()
	void OnRankSortButtonClicked();

	UFUNCTION()
	void OnChuckButtonClicked();

	UFUNCTION()
	void OnHandPlayButtonClicked();

private:
	bool SetCardData(OUT TArray<FDeckCardStat>& CardStatInfo, OUT int32& SelectedCardNum);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SuitSortButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RankSortButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> HandPlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ChuckButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> CardPanel;

	UPROPERTY()
	TArray<TObjectPtr<class UCardButton>> HandCardButton;


	int32 CardIndex = 0;
};
