// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/Core/BBUserWidgetBase.h"
#include "GameData/HandRankingStat.h"
#include "GameData/DeckCardStat.h"
#include "CardDeckView.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BALRATRO_API UCardDeckView : public UBBUserWidgetBase
{
	GENERATED_BODY()
public:
	UCardDeckView();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

protected:
	void VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_CardUpExist(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_CurPlayCardData(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_ItemSelectFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_RestCardData(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_CurBossUseSkill(UObject* Object, UE::FieldNotification::FFieldId FieldId);
	void VM_FieldChanged_CurBossText(UObject* Object, UE::FieldNotification::FFieldId FieldId);

	void VM_FieldChanged_UselessBlindEmblem(UObject* Object, UE::FieldNotification::FFieldId FieldId);

private:
	UFUNCTION()
	void OnSuitSortButtonClicked();
	UFUNCTION()
	void OnRankSortButtonClicked();
	UFUNCTION()
	void OnChuckButtonClicked();
	UFUNCTION()
	void OnHandPlayButtonClicked();

private:
	void	CardScroe_EffectText(); // 점수 관련 이벤트들

	void	SetCard_PrevEffectOrder(class UCardButtonWidget* EventCard, FDeckCardStat& CardData); // 맨앞에 효과
	void	SetCard_EffectOrder(class UCardButtonWidget* EventCard, FDeckCardStat& CardData);     // 중간 효과
	void	SetRestCard_EffectOrder(class UCardButtonWidget* EventCard, FDeckCardStat& CardData); // 맨뒤 효과

	// 순서 BaseChip >  Chip_Plus or Drainage > 10배수 or 1.5 or 50(foil) > Ghost (retriger)
	void SetScoreTextPos(class UCardButtonWidget* CurEventCard, bool IsUp = false);

	void	StartNextTimer();
	void	PushTimerEvent(TFunction<void(class UCardButtonWidget*,int32)> InFunc, class UCardButtonWidget* CurEventCard,int32 InValue);

private:
	bool  SetCardData(OUT TArray<UHandInCard_Info*>& CardStatInfo, OUT int32& SelectedCardNum);
	class UCardButtonWidget* ReuseCardButtonWidget(int32 CurAllCardNum, int32 CurNum, UHandInCard_Info* CardInfo);

	FString		GetBossSkillText(EBossType _InType);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPaenl;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SuitSortButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> RankSortButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> HandPlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ChuckButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> CardPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ScoreResultText;

	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> HandSortBorder;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HandSortText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ChuckButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BossSkillText;

private:
	UPROPERTY()
	TSubclassOf<class UCardButtonWidget> CardButtonSubClass;

	UPROPERTY()
	TArray<TObjectPtr<class UCardButtonWidget>> HandCardButtons;

private:
	const int32 PaddingX = 8;
	int32 CardIndex = 0;
	
	FTimerHandle MyTimerHandle;

	TQueue<FTimerDelegate> TimerFuncQueue;


	FTimerHandle FinishScoreTimerHandle;
};
