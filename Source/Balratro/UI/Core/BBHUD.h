// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BBHUD.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API ABBHUD : public AHUD
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	class UUserWidget* MainMenuWidget;

};
