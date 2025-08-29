// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/BlindStat.h"
#include "Item/BoosterPackData.h"
#include "GameData/JokerStat.h"
#include "GameData/BoucherStat.h"
#include "StoreComponent.generated.h"

USTRUCT()
struct FItemWeight
{
	GENERATED_BODY()

	EBoosterPackType Type;
	int32 Weight;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UStoreComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void	SetStoreView(EPlayerStateType _InType);
	void	ReRollCostUp();
	void	StartBoosterPackEvent(UBoosterPackData* InData);

private:	
	void	SetUpStoreItem();
	void	SetBoucherItem();
	void	SetDownStoreItem();
	

	void	EraseStoreBoucherCard(FBoucherInfo& _Info);

	void	InitStoreData();

private:
	EBoosterPackType	SetItemType();
	EJokerType			SetJokerType();

private:
	class UVM_PlayerInfo*	GetVMPlayerInfo();
	class UVM_Store*		GetVMStore();
	class AMyPlayerState*	GetPlayerState();
	class UVM_MainMenu*		GetVMMainWidget();

private:
	TArray<FItemWeight> ItemWeights;  

	// 상점이 가지고 있어야되는 이유 플레이어가 아직 소유한 것이 아닌 상점소유 데이터라서
	UPROPERTY()
	TArray<class UBoosterPackData*> BoosterPacks; 

	UPROPERTY()
	TArray<class UJokerCard_Info*> JokerCards;

	//UPROPERTY()
	//TObjectPtr<class UBoucherStat> BoucherDataAsset;


	TArray<FBoucherInfo> BoucherInfos;


	int32 BoosterPackIndex = 0;
};
