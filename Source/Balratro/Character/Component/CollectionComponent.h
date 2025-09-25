// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/BoucherStat.h"
#include "GameData/JokerStat.h"

#include "CollectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCollectionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void		InitCollectionData();

private:
	void	 AddJokerCollection(FJokerStat& Data, bool IsAdd);
	void	 AddBoucherCollection(FBoucherInfo Data);

	void	 ActiveCollectionView();


private:
	class UVM_Collection* GetVMCollection();
	class AMyPlayerState* GetPlayerState();
	class UVM_MainMenu* GetVMMainWidget();
	class UVM_JockerSlot* GetVMJockerSlot();

private:
	UPROPERTY()
	TMap<class UJokerCard_Info* ,bool> JokerCollection;

	UPROPERTY()
	TArray<class UTaroStat_Info*>  TaroStatCollection;

	UPROPERTY()
	TMap<EBoucherType,bool>		   BoucherCollection;
};
