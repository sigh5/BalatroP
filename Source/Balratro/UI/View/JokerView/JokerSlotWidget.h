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
	void VM_FieldChanged_CalcualtorJokerCard(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_PlayJokerEvent(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_PreJokerEventStopFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_RoundFinishEvent(UObject* Object, UE::FieldNotification::FFieldId FieldId);


private:
	class UJokerCardWidget* ReuseCardButtonWidget(int32 AllNum, int32 Index, UJokerCard_Info* Data);

private:

	void	JokerScroe_EffectText(); // 점수 관련 이벤트들
	void	SetJoker_EffectOrder(class UJokerCardWidget* EventJoker, FJokerStat& CardData);
	void	PushTimerEvent(TFunction<void(class UJokerCardWidget*, FJokerStat)> InFunc, class UJokerCardWidget* CurEventJoker, FJokerStat InValue);

	void	StartNextTimer();

	void	ResetJoker();

	void SetSkillTextPos(class UJokerCardWidget* CurEventCard);
	void SetSkillTextPos2(class UJokerCardWidget* CurEventCard);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> JokerWrapBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SkillText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SkillText2;

private:
	UPROPERTY()
	TSubclassOf<class UJokerCardWidget> JokerCardSubClass;

	UPROPERTY()
	TArray<TObjectPtr<class UJokerCardWidget>> JokerButtons;

private:
	const int32 PaddingX = 8;
	TQueue<FTimerDelegate> TimerFuncQueue;

	FTimerHandle		   JokerEffectTimerHandle;
	FTimerHandle			FinishJokerTimerHandle;
};
