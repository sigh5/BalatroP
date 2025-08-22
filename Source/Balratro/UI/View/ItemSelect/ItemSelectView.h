// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/TaroStat.h"
#include "ItemSelectView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UItemSelectView : public UBBUserWidgetBase
{
	GENERATED_BODY()
public:
	UItemSelectView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	void VM_FieldChanged_TaroList(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	class UItemCardWidget* ReUseBoosterPackWidget(int32 DataNum, int32 Index, FTaroStat& Data);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ItemPanel;

	UPROPERTY()
	TSubclassOf<class UItemCardWidget> TaroStatSubClass;

private:
	UPROPERTY()
	TArray<class UItemCardWidget*> BoosterPackWidgets;
};
