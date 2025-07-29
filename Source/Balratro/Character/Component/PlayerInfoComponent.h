// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/MyPlayerState.h"
#include "PlayerInfoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UPlayerInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void UpdateRoundCount(int16 _invalue);
	void UpdateGold(int16 _invalue);
	void UpdateEntiCount(int16 _invalue);
	void UpdateMaxHandCount(int16 _invalue);
	void UpdateUseHandCount(int16 _invalue);
	void UpdateUseChuckCount(int32 _Invalue);
	void UpdateCurrentScore(int32 _invalue);
	void UpdateMaxScore(int32 _invalue);

	void UpdateHandRanking();


protected:
	virtual void BeginPlay() override;

private:
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class AMyPlayerState* GetPlayerState();



};
