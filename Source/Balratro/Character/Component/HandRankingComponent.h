// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/CalculatorScoreInterface.h"
#include "HandRankingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UHandRankingComponent : public UActorComponent, public ICalculatorScoreInterface
{
	GENERATED_BODY()


public:
	virtual EPokerHand CalCulatorHandRanking(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat) override;

protected:
	virtual void BeginPlay() override;

private:
	void	CalculatorBaseScore(EPokerHand HandRankingType, TArray<FDeckCardStat>& _DeckCardStat);

private:
	class UVM_HandRankingCount* GetVMHandRanking();
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class AMyPlayerState* GetPlayerState();
	

};
