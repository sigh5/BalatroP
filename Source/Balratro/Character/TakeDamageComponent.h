// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TakeDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UTakeDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OnTakeDamageTick();

	void UpdateMaxHealth(int32 NewMaxHealth);
	void UpdateCurrentHealth(int32 NewCurrentHealth);

private:
	class UVM_MainMenu* GetVMPlayerHealth();
	class AMyPlayerState* GetPlayerState();
	FTimerHandle TakeDamageHandle;
		
};
