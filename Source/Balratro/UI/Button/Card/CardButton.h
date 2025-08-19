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
	
	void			SetCardInfoData(FDeckCardStat& InValue);
	FORCEINLINE FDeckCardStat&	GetCardInfoData() {return CardInfoData; }

	FORCEINLINE const bool	GetSelected() const { return bSelected; }
	FORCEINLINE void		SetSelected(bool _Selected) { bSelected = _Selected; }

	void					SetClikcedEvent();
	
	
private:
	UFUNCTION()
	void OnCardButtonClicked();

private:
	void					ChangeImage();
	void					CreateImage();

	class UVM_CardDeck* GetVMCardDeck();
private:
	int CardIndex = 0;
	FDeckCardStat CardInfoData;
	
	uint8 bSelected : 1;

private:

	UPROPERTY()
	TObjectPtr<class UBorder> Border;

	UPROPERTY()
	TObjectPtr<class UImage> Image;


};
