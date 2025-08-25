// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/JokerStat.h"
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

private:
	void VM_FieldChanged_AddJokerCard(UObject* Object, UE::FieldNotification::FFieldId FieldId);


private:
	class UJokerCardWidget* ReuseCardButtonWidget(int32 AllNum, int32 Index, UJokerCard_Info* Data);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> JokerWrapBox;

private:
	UPROPERTY()
	TSubclassOf<class UJokerCardWidget> JokerCardSubClass;

	UPROPERTY()
	TArray<TObjectPtr<class UJokerCardWidget>> JokerButtons;

private:
	const int32 PaddingX = 8;
};
