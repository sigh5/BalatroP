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

protected:
    void VM_FieldChanged_WidgetName(UObject* Object, UE::FieldNotification::FFieldId FieldId);
   

private:
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* CanvasSlot;


    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> BackGroundImage;

    UPROPERTY()
    TSubclassOf<class UBlindSelectView> BlindSelectView;

    UPROPERTY()
    TSubclassOf<class UPlayerInfoWidget> PlayerInfoView;

    UPROPERTY()
    TSubclassOf<class UJokerSlotWidget> JokerSlotView;

    UPROPERTY()
    TSubclassOf<class UCardDeckView> CadDeckView;

    UPROPERTY()
    TSubclassOf<class UStoreView> StoreView;


    UPROPERTY()
    TObjectPtr<class UBBUserWidgetPool> WidgetPool;


};
