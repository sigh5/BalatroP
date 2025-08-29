// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/BoucherStat.h"
#include "HRBoucherView.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UHRBoucherView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UHRBoucherView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	void VM_FieldChanged_ShowBoucherCards(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	class UBoucherCardWidget* ReUseBoucherWidget(int32 DataNum, int32 Index, FBoucherInfo& Data);

private:
	UPROPERTY()
	TSubclassOf<class UBoucherCardWidget> BoucherCardWidgetSubClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> BoucherWrapBox;

private:

	UPROPERTY()
	TArray<class UBoucherCardWidget*> BoucherCardWidgets;

};
