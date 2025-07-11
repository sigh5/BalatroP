// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Blueprint/IUserObjectListEntry.h>
#include "HandRankingListView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UHandRankingListView : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();

	class UVM_HandRankingCount* GetVMHandRanking();

private:
	void UpdateVisuals();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ChipText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> drainageText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NumTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> InfoButton;

	

	UPROPERTY()
	TObjectPtr<class UHandRanking_Info> MemberData;
};
