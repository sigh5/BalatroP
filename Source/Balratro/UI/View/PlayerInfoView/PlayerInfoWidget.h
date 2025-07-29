// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UPlayerInfoWidget : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UPlayerInfoWidget();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_CurPlayerHandName(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	
	void VM_FieldChanged_CurPlayerChuckCount(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_CurPlayerHandCount(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	
	void VM_FieldChanged_CurHandRanking_Chip(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_CurHandRanking_Level(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_CurHandRanking_Drainage(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_DeckNum(UObject* Object, UE::FieldNotification::FFieldId FieldId);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MainOrderText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> UseHandNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ChipText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DrainageText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HandCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ChuckCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> EntiText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RoundText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentDeckNumText;
};
