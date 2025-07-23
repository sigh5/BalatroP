// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core\MyPlayerState.h"
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

	void UpdateChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat);
	void SetHandPlay(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat);


protected:
	virtual void BeginPlay() override;
	
private:
	void	InitDeck();
	
	void	SortHandInCard(const EHandInCardSortType& InType);
	
	void	SortRank();
	void	SortSuit();


	

private:
	class UVM_CardDeck* GetVMCardDeck();
	class AMyPlayerState* GetPlayerState();

	int32  CurIndex = 0;

	
};
