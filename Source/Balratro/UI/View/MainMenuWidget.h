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
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;
    
protected:
    void VM_FieldChanged_WidgetName(UObject* Object, UE::FieldNotification::FFieldId FieldId);
    void VM_FieldChanged_ClearAnimFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId);
    void VM_FieldChanged_MainLogoFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
    UFUNCTION()
    void    OnMain_PlayButtonClicked();

    UFUNCTION()
    void    OnMain_OptionButtonClicked();

    UFUNCTION()
    void    OnMain_ExitButtonClicked();

    UFUNCTION()
    void    OnMain_CollectionClicked();


private:
    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* CanvasSlot;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> BackGroundImage0;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> BackGroundImage1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> LogoImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> BackGroundImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> ClearBorder2;


    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UHorizontalBox> ButtonBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> PlayButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> OptionButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> CollectionButton;




    UPROPERTY(meta = (BindWidgetAnim), Transient)
    TObjectPtr<class UWidgetAnimation> ClearAnimation;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    TObjectPtr<class UWidgetAnimation> LogoAnimation;

private:
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
    TSubclassOf<class URewardView> RewardView;

    UPROPERTY()
    TSubclassOf<class UItemSelectView> ItemSelectView;

    UPROPERTY()
    TSubclassOf<class UHandRankingView> HandRankingView;

    UPROPERTY()
    TSubclassOf<class UGameOverView> GameOverView;

    UPROPERTY()
    TSubclassOf<class UCollectionView> CollectionView;

    UPROPERTY()
    TObjectPtr<class UBBUserWidgetPool> WidgetPool;


};
