// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "RewardView.generated.h"

/**
 * 
 */
UCLASS()
class BALRATRO_API URewardView : public UBBUserWidgetBase
{
	GENERATED_BODY()
	
public:
	URewardView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnCashOutButton();

	void StartQueueAnimation();

protected:
	void VM_FieldChanged_BlindRewardText(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_RestHands(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_Interest(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_GoldJoker(UObject* Object, UE::FieldNotification::FFieldId FieldId);


	void VM_FieldChanged_BlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_BlindImageIndex(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_EarnGold(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void UpdateDollarAnimation(class UTextBlock* numberText, class UTextBlock* strText, int32 MaxNum);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CashOutButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BlindImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BlindGradeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BlindReward;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HandReward; //$

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> RestHand; // 숫자

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Interrest; // 숫자

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InterestReward;  // $
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DotLine;  // $
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldJokerReward;  // $

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldJokerText;  

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldJoker;  // 숫자



	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> GoldTextAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> DotLineAnim;

	FTimerHandle DollarAnimTimer;
	int32 AnimStep = 0;

	TQueue<FTimerDelegate> StartQueue;
	
	int32 RewardStep = 0;
	int32 RestHandStep = 0;
	int32 interestStep = 0;
	int32 GoldJokerStep = 0;
	uint8 isFirst : 1;
	
};
