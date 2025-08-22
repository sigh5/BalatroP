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
	void	DrawCard(int32 DrawCardNum); //		현재 모델에 있는 카드 데이터를 VM에 전달
	void	SortHandInCard(const EHandInCardSortType& InType);
	// 핸드플레이, 버리기 등 현재 손패에 있는 카드 데이터 제거 후 모델에 전달
	void	UpdateCardInHand(TArray<FDeckCardStat>& _DeckCardStat); 

	void	SetVisibleCardDeckView(EPlayerStateType InValue);
	void    FinishHandPlay();

private:
	void	UseTaroItem(FTaroStat& TaroStat);
	
	void	UseEnhanceTaro(int32 EnhanceType);
	void	UseSealTaro(int32 SealType);
	void	UseGhostTaro(int32 GhostType);



private:
	class UVM_CardDeck*		GetVMCardDeck();
	class AMyPlayerState*	GetPlayerState();
	class UVM_MainMenu*		GetVMMainWidget();
	class UVM_ItemSelect* GetVMItemSelect();


private:
	int32  CurDrawIndex = 0;
	TArray<FDeckCardStat> _CurData; // 타이머를 위해 저장하는 단순 변수
	int32 _CardNum = 0;
	float _DelayTime = 0.f;
	int32 ResultScore = 0;

	FTimerHandle TotalScoreHandle;
};
