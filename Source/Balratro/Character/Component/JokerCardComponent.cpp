// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/JokerCardComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

void UJokerCardComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UJokerCardComponent::UpdateJoker()
{
	


}

void UJokerCardComponent::CalculatorJoker(UJokerCard_Info* JokerCard)
{
	auto PS = GetPlayerState();
	auto VM_PlayerInfo = GetVMPlayerInfo();

	int32 CurrentChip = PS->GetCurrentShowChip();
	int32 CurrentDrainage = PS->GetCurrentShowDrainage();

	JokerType Type = JokerType::BASE_JOKER;		//(JokerType)(JokerCard->Info.Stat);
	if (Type == JokerType::BASE_JOKER)
	{
		VM_PlayerInfo->SetCurDrainage(CurrentDrainage * 4);
	}
}

UVM_JockerSlot* UJokerCardComponent::GetVMJockerSlot()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_JockerSlot");
	Context.ContextClass = UVM_JockerSlot::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_JockerSlot>(Found);
}

UVM_PlayerInfo* UJokerCardComponent::GetVMPlayerInfo()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_PlayerInfo");
	Context.ContextClass = UVM_PlayerInfo::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_PlayerInfo>(Found);
}

AMyPlayerState* UJokerCardComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}
