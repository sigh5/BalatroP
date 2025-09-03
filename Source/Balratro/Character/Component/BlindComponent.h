// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/BlindStat.h"
#include "BlindComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UBlindComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void	InitBlindSelectView();
	void	BlindSelectEvent(EPlayerStateType InValue);
	void	BlindViewActive();
	void	ResetBlindSelectData();
	
	void	SetRandomBossType();
	void	UseBossSkill();

	void	SetBlindSkipReward();


private: /* Skill */
	void	HOOK_Skill();
	void	OX_Skill();
	void	WALL_SKill();
	void	ARM_Skill();
	void	PSYCHIC_Skill();
	void	GOAD_Skill();
	void	WATER_Skill();
	void    EYE_Skill();

private:
	class UVM_BlindSelect* GetVMBlindSelect();
	class UVM_MainMenu* GetVMMainWidget();
	class AMyPlayerState* GetPlayerState();
	class UVM_Store* GetVMStore();
	class UVM_CardDeck* GetVMCardDeck();
private:
	//UPROPERTY()
	TMap<EBossType, TFunction<void()>> BossTypes;

	TArray<int32> RandomArray;

};
