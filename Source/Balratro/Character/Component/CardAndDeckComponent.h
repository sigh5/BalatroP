// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core\MyPlayerState.h"
#include "CardAndDeckComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCardAndDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void UpdateChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat);
	void UpdateHandPlay(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat);

	void	SetPlayCardEffect();

protected:
	virtual void BeginPlay() override;
	
private:
	void	InitDeck();
	void	ShuffleDeck();

	//		���� �𵨿� �ִ� ī�� �����͸� VM�� ����
	void	DrawCard(int32 DrawCardNum);


	void	SortHandInCard(const EHandInCardSortType& InType);

	// �ڵ��÷���, ������ �� ���� ���п� �ִ� ī�� ������ ���� �� �𵨿� ����
	void	UpdateCardInHand(TArray<FDeckCardStat>& _DeckCardStat);

	void	SetVisibleCardDeckView(EPlayerStateType InValue);

	void    FinishHandPlay();

private:
	class UVM_CardDeck*		GetVMCardDeck();
	class AMyPlayerState*	GetPlayerState();
	class UVM_MainMenu*		GetVMMainWidget();

	int32  CurDrawIndex = 0;
	TArray<FDeckCardStat> _CurData;
	int32 _CardNum = 0;
	float _DelayTime = 0.f;
	int32 ResultScore = 0;

	FTimerHandle TotalScoreHandle;
};
