// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "GameData/DeckCardStat.h"
#include "CardButton.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UCardButton : public UButton
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetCardIndex(int InValue) { CardIndex = InValue; }
	
	FORCEINLINE void SetCardInfoData(FDeckCardStat& InValue) { CardInfoData = InValue; }
	FORCEINLINE FDeckCardStat& GetCardInfoData() {return CardInfoData; }

	void SetClikcedEvent();

	UFUNCTION()
	void OnCardButtonClicked();


	FORCEINLINE const bool GetSelected() const { return bSelected; }
	FORCEINLINE void SetSelected(bool _Selected) { bSelected = _Selected; }

	class UVM_CardDeck* GetVMCardDeck();

	void	SetImage();

	FORCEINLINE void SetButtonBorder(class UBorder* InValue) { Border = InValue; }
	FORCEINLINE void SetButtonImage(class UImage* InValue) { Image = InValue; }


private:
	int CardIndex = 0;
	FDeckCardStat CardInfoData;
	
	uint8 bSelected : 1;

	UPROPERTY()
	TObjectPtr<class UBorder> Border;

	UPROPERTY()
	TObjectPtr<class UImage> Image;


};
