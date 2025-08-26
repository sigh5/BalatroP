// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JokerCalculatorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJokerCalculatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BALRATRO_API IJokerCalculatorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CalculatorJokerSkill(OUT int32& CurChip, OUT float& CurDriange) = 0;
};
