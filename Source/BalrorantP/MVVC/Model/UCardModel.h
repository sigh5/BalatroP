// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UCardModel.generated.h"

/**
 * 
 */
UCLASS()
class BALRORANTP_API UUCardModel : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FText Name;

	UPROPERTY(BlueprintReadOnly)
	int32 Value;

	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color;

	void initFromModel(UUCardModel* Model);


};
