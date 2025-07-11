// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "JokerSlotWidget.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UJokerSlotWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UJokerSlotWidget();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_Players(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> PartyListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StatusTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> AddButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RemoveButton;

	UFUNCTION()
	void AddClicked();
	UFUNCTION()
	void RemoveClicked();
	
	int32 TotalAddedCount = 0;

};
