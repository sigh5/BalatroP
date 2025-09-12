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
	virtual void LastCalculatorJokerSkill(OUT int32& CurChip, OUT float& CurDriange) override;
	virtual void CalculatorCardJokerSkill(class UHandInCard_Info* CurCard, OUT int32& CurChip, OUT float& CurDriange) override;
	virtual void FinishRoundJokerSkill() override;
protected:
	virtual void BeginPlay() override;

private:
	void UpdateAddJoker(FJokerStat& Data,bool IsAdd);

	void LastCalculatorJoker(UJokerCard_Info* JokerCard ,OUT int32& CurChip , OUT float& CurDriange);
	void PlayCalculatorJoker(UJokerCard_Info* JokerCard, class UHandInCard_Info* CurCard, OUT int32& CurChip, OUT float& CurDriange);

	void UpdateSwapJokerData(UJokerCard_Info* Source, UJokerCard_Info* SwapDest);

private:
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class UVM_JockerSlot* GetVMJockerSlot();
	class AMyPlayerState* GetPlayerState();


private:
	int32 JokerGoldSum = 0;
	uint8 IsCopy : 1;
};
