// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/BBHUD.h"
#include "Blueprint/UserWidget.h"

void ABBHUD::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass); //GetOwningPlayerController
	MainMenuWidget->AddToViewport();

	auto InputMode = FInputModeUIOnly();
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetOwningPlayerController()->SetInputMode(InputMode);

	GetOwningPlayerController()->bShowMouseCursor = true;
}
