// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardAndDeckComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCardAndDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void UpdateCardInHand(int32 _invalue);
	void UpdateCardInDeck();

	void ShuffleDeck();
	void DrawCard(int32 DrawCardNum);
	
	void	SortRank();
	void	SortSuit();
protected:
	virtual void BeginPlay() override;
	
private:
	void	InitDeck();

	

private:
	class UVM_CardDeck* GetVMCardDeck();
	class AMyPlayerState* GetPlayerState();

	int32  CurIndex = 0;

};
