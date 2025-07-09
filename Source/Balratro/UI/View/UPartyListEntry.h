// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Blueprint/IUserObjectListEntry.h>
#include "UPartyListEntry.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UUPartyListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	void UpdateVisuals();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameTextBlock;

	UPROPERTY()
	TObjectPtr<class UPartJoker> MemberData;
};
