// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInfoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UPlayerInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	int32 Update

protected:
	virtual void BeginPlay() override;

private:
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class AMyPlayerState* GetPlayerState();


};
