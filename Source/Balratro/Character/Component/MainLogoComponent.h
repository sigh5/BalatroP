// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/EnumDatas.h"
#include "MainLogoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UMainLogoComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void  BlindSelctViewStart();
	void  InitLogoView(EPlayerStateType _InValue);

private:
	class UVM_BlindSelect* GetVMBlindSelect();
	class UVM_MainMenu* GetVMMainWidget();
	class AMyPlayerState* GetPlayerState();
	class UVM_Store* GetVMStore();
	class UVM_CardDeck* GetVMCardDeck();
		
};
