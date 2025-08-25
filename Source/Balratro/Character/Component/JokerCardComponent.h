// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/JokerStat.h"
#include "JokerCardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UJokerCardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;



private:
	void UpdateJoker();
	void CalculatorJoker(UJokerCard_Info* JokerCard);

	void UpdateAddJoker(FJokerStat& Data);


private:
	class UVM_PlayerInfo* GetVMPlayerInfo();

	class UVM_JockerSlot* GetVMJockerSlot();

	class AMyPlayerState* GetPlayerState();

};
