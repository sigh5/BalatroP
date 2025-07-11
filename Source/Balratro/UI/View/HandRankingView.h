// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "HandRankingView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UHandRankingView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UHandRankingView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void VM_FieldChanged_Status(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_WidgetPos(UObject* Object, UE::FieldNotification::FFieldId FieldId);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> HandRankingListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUserWidget> HandRankingToolTipWidget;

	UPROPERTY()
	int RoyalFlushNum = 0;


};
