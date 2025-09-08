// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/EnumDatas.h"
#include "RewardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BALRATRO_API URewardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void    SetRewardViewData(EPlayerStateType InType);

	void	StartStoreView();

private:
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu* GetVMMainWidget();
	class UVM_PlayerInfo* GetVMPlayerInfo();
	class UVM_Reward* GetVMReward();

private:
	int32 EarnGold = 0;

};
