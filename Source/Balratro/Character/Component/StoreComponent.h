// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/BlindStat.h"
#include "StoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UStoreComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void	SetStoreView(EPlayerStateType _InType);

private:
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu* GetVMMainWidget();
};
