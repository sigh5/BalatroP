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

private:
	TSet<int32>	 SetTaroType();


private:
	class UVM_Store* GetVMPStore();
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu* GetVMMainWidget();
	class UVM_ItemSelect* GetVMItemSelect();
	class UVM_CardDeck* GetVMCardDeck();

private:
	TArray<FTaroStat>   CurShowTaroInfo; // UI 에 보여지는 타로 리스트

};
