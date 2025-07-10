// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	class UTextBlock* GetTextBlock() { return TestText; }

public:
	virtual void NativeConstruct() override;


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TestText;

};
