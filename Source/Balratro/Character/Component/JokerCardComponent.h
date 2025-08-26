// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/JokerStat.h"
#include "Interface/JokerCalculatorInterface.h"
#include "JokerCardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UJokerCardComponent : public UActorComponent, public IJokerCalculatorInterface
{
	GENERATED_BODY()

public:
	virtual void CalculatorJokerSkill(OUT int32& CurChip, OUT float& CurDriange) override;

protected:
	virtual void BeginPlay() override;



private:
	void UpdateJoker();
	void UpdateAddJoker(FJokerStat& Data);

	void CalculatorJoker(UJokerCard_Info* JokerCard ,OUT int32& CurChip , OUT float& CurDriange);

private:
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class UVM_JockerSlot* GetVMJockerSlot();
	class AMyPlayerState* GetPlayerState();

};
