// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "WidgetPosMoveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetPosMoveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALRATRO_API IWidgetPosMoveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetHandRanking_ToolTipWidgetPos(const FHRButton_Info& _Info, const FVector2D& Pos) = 0;
};
