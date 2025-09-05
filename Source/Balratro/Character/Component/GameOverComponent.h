// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/BlindStat.h"
#include "GameOverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UGameOverComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void	 SetGameOverView(EPlayerStateType InType);

	void	SetNewRunEvent();

private:
	class UVM_Store* GetVMStore();
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu* GetVMMainWidget();
	class UVM_GameOver* GetVMGameOver();
	class UVM_BlindSelect* GetVMBlindSelect();

	
};
