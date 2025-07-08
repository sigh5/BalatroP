// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BBGameMode.h"
#include "UI/Core/BBHUD.h"


ABBGameMode::ABBGameMode()
{
	HUDClass = ABBHUD::StaticClass();
	//PlayerStateClass = AMyPlayerState::StaticClass();
	//PlayerControllerClass = AMyPlayerController::StaticClass();
	//DefaultPawnClass = AMyPawn::StaticClass();
}
