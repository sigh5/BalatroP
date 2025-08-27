// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/CalculatorScoreInterface.h"
#include "HandRankingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UHandRankingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void InitHandRanking();

	void ExitHandRankingView();
private:
	class UVM_HandRankingCount* GetVMHandRanking();
	class AMyPlayerState* GetPlayerState();
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class UVM_MainMenu* GetVMMainWidget();
};
