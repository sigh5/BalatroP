// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/WidgetPosMoveInterface.h"
#include "ToolTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API UToolTipWidget : public UUserWidget , public IWidgetPosMoveInterface
{
	GENERATED_BODY()
	
public:
	class UTextBlock* GetTextBlock() { return TestText; }

public:
	virtual void NativeConstruct() override;

	virtual void SetWidgetPos(FVector2D& Pos) override;


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TestText;

};
