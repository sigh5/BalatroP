// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/BBHUD.h"
#include "Blueprint/UserWidget.h"

void ABBHUD::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuWidgetClass); //
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create MainMenuWidget!"));
		return;
	}
	
	MainMenuWidget->AddToViewport();

	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuWidgetClass is null!"));
		return;
	}

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null!"));
		return;
	}

	auto InputMode = FInputModeUIOnly();
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetOwningPlayerController()->SetInputMode(InputMode);

	GetOwningPlayerController()->bShowMouseCursor = true;
}
