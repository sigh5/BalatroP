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

	void	InitBlindSelectView();
	
	void	BlindSelectEvent(EPlayerStateType InValue);

	void	BlindViewActive();

private:
	class UVM_BlindSelect* GetVMBlindSelect();
	class UVM_MainMenu* GetVMMainWidget();
	class AMyPlayerState* GetPlayerState();
	class UVM_Store* GetVMStore();

private:

};
