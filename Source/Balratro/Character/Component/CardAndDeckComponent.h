// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/TaroStat.h"
#include "GameData/EnumDatas.h"
#include "CardAndDeckComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCardAndDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void UpdateChuck(int32 CardNum, TArray<class UHandInCard_Info*>& _DeckCardStat);
	void UpdateHandPlay(int32 CardNum, TArray<class UHandInCard_Info*>& _DeckCardStat);

	void	SetPlayCardEffect();

protected:
	virtual void BeginPlay() override;
	
private:
	void	InitDeck();
	void	ShuffleDeck();
	void	DrawCard(int32 DrawCardNum); //		���� �𵨿� �ִ� ī�� �����͸� VM�� ����
	void	SortHandInCard(const EHandInCardSortType& InType);
	// �ڵ��÷���, ������ �� ���� ���п� �ִ� ī�� ������ ���� �� �𵨿� ����
	void	UpdateCardInHand(TArray<class UHandInCard_Info*>& _DeckCardStat);

	void	SetVisibleCardDeckView(EPlayerStateType InValue);
	void    FinishHandPlay();

	void	SwapCardOrder(UHandInCard_Info* SwapDest, UHandInCard_Info* Source);

private:
	void	UseTaroItem(FTaroStat& TaroStat);
	
	void	UseEnhanceTaro(int32 EnhanceType);
	void	UseSealTaro(int32 SealType);
	void	UseGhostTaro(int32 GhostType);

	void	AllEffectViewFinish();

private:
	class UVM_CardDeck*		GetVMCardDeck();
	class AMyPlayerState*	GetPlayerState();
	class UVM_MainMenu*		GetVMMainWidget();
	class UVM_ItemSelect* GetVMItemSelect();
	class UVM_JockerSlot* GetVMJoker();

private:
	int32  CurDrawIndex = 0;
	
	//int32 _CardNum = 0;
	float _DelayTime = 0.f;
	int32 ResultScore = 0;

	FTimerHandle TotalScoreHandle;
	FTimerHandle ItemSkipTimerHandle;


	UPROPERTY()
	TArray<UHandInCard_Info*> DeckCardStat;
	
	int32 nCardNum;

	//UPROPERTY()
	//TArray<class UHandInCard_Info*> _CurData; // Ÿ�̸Ӹ� ���� �����ϴ� �ܼ� ����
};
