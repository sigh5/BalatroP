// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/CalculatorScoreInterface.h"
#include "GameData/EnumDatas.h"
#include "CalculatorHandRankingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCalculatorHandRankingComponent : public UActorComponent, public ICalculatorScoreInterface
{
	GENERATED_BODY()

public:
	virtual int32 CalCulatorHandRanking(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat) override;

protected:
	virtual void BeginPlay() override;

private:
	void	SetHandRankName(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat);
	void	SetHandRankingType(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat);
	
	void	SetCalRestCardInHands(TArray<UHandInCard_Info*>& _CurPlayCards);
	void	SetStillCards();

	void	Calculator_StillCard(OUT int32& CurChip, OUT float& CurDriange); // CurDriange이거 때문에
	void	Calculator_Joker(OUT int32& CurChip, OUT float& CurDriange);

	int32	ResultScore();

	void	ResetComponentInfo(EPlayerStateType InValue);

private:
	bool	IsStraight(TArray<int32>& SortedRanks);
	bool	IsFlush(TMap<int32, int32>& SuitCounts);

private:
	class UVM_CardDeck* GetVMCardDeck();
	class AMyPlayerState* GetPlayerState();

	UPROPERTY()
	TArray<UHandInCard_Info*> CurStillCard;

};
