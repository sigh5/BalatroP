// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/BlindStat.h"
#include "GameData/TaroStat.h"
#include "ItemSelectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UItemSelectComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void	SetItemSelectView(EPlayerStateType _InType);
	
	void	SetItemList();

	void    InitTaroInfoTable();
	void	ItemSelectSceneSkip();


	void	SetBlindSkipReward(EBlindSkip_Tag CurTagType);

	void	BlindSkipReward_BoosterPackSetting(EBlindSkip_Tag CurTagType);

	void	SetBlindSkipButtonFlag(EPlayerStateType InType);

private:
	TSet<int32>	 SetTaroType(int32 SetTaroNum);


private:
	class UVM_Store* GetVMStore();
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu* GetVMMainWidget();
	class UVM_ItemSelect* GetVMItemSelect();
	class UVM_CardDeck* GetVMCardDeck();
	class UVM_BlindSelect* GetVMBlindSelect();

private:
	TArray<FTaroStat>   CurShowTaroInfo; // UI 에 보여지는 타로 리스트

	uint8 IsClickedBlindSkip : 1;

};
