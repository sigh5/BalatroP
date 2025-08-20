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
	void UpdateRoundCount();
	void UpdateEntiCount();
	void UpdateGold();
	
	void UpdateCardInDeck();
	void UpdateMaxHandCount(int16 _invalue);
	void UpdateUseHandCount(int32 _invalue);
	void UpdateUseChuckCount(int32 _Invalue);
	void UpdateCurrentScore();
	void UpdateMaxScore(int32 _invalue);

	void UpdateHandRanking();
	void UpdateBlindInfo(EPlayerStateType _InType);

	void UpdateCalculatorChip();
	void UpdateCalculatorDrainage();

protected:
	virtual void BeginPlay() override;

private:
	void  Init_PlayerInfo();

private:
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu*	  GetVMMainWidget();



};
