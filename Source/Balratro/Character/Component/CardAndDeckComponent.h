// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardAndDeckComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BALRATRO_API UCardAndDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void UpdateCardInHand(int32 _invalue);
	void UpdateCardInDeck(int32 _invalue);


protected:
	virtual void BeginPlay() override;
		
private:
	class UVM_CardDeck* GetVMCardDeck();
	class AMyPlayerState* GetPlayerState();

};
