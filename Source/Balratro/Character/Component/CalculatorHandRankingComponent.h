// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/CalculatorScoreInterface.h"
#include "CalculatorHandRankingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCalculatorHandRankingComponent : public UActorComponent, public ICalculatorScoreInterface
{
	GENERATED_BODY()

public:
	virtual int32 CalCulatorHandRanking(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) override;

protected:
	virtual void BeginPlay() override;

	void	SetHandRankName(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat);
	void	SetHandRankingType(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat);

	void	SetCurItemUseCards(TArray<FDeckCardStat>& _DeckCardStat);

private:
	bool	IsStraight(TArray<int32>& SortedRanks);
	bool	IsFlush(TMap<int32, int32>& SuitCounts);

	int32	ResultScore();



private:
	class UVM_CardDeck* GetVMCardDeck();
	class AMyPlayerState* GetPlayerState();

};
