// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UMainMenuWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget();

protected:
    void NativeConstruct() override;

private:
    void OnFieldChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId);

    void OnFieldTextChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId);


  /*  UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UProgressBar> HealthProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> TestText;*/

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> BackGroundImage;


};
