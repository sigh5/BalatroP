// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "StoreView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UStoreView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UStoreView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	void VM_FieldChanged_BoosterPacks(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_UpStoreJoker(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_UpStoreJoker(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	UFUNCTION()
	void OnNextButton();

	UFUNCTION()
	void OnReRollButton();

private:
	class UBoosterPackWidget* ReUseWidget(int32 DataNum, int32 Index,class UBoosterPackData* Data);

	class UJokerCardWidget* ReUseJokerWidget(int32 DataNum, int32 Index, class UJokerCard_Info* Data);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> NextButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReRollButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ReRollText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> UpHorizontalBox;	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> BoucherHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> PackHorizontalBox;

	UPROPERTY()
	TSubclassOf<class UBoosterPackWidget> BoosterPackWidgetSubClass;

	UPROPERTY()
	TSubclassOf<class UJokerCardWidget> JokerCardWidgetSubClass;

private:
	UPROPERTY()
	TArray<class UBoosterPackWidget*> BoosterPackWidgets;

	UPROPERTY()
	TArray<class UJokerCardWidget*> JokercardWidgets;

};
