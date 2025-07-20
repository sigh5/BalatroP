#include "Character/Component/PlayerInfoComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "Core/MyPlayerState.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

void UPlayerInfoComponent::BeginPlay()
{
	Super::BeginPlay();


	
}

UVM_PlayerInfo* UPlayerInfoComponent::GetVMPlayerInfo()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_PlayerInfo");
	Context.ContextClass = UVM_PlayerInfo::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_PlayerInfo>(Found);
}

AMyPlayerState* UPlayerInfoComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}

void UPlayerInfoComponent::UpdateRoundCount(int16 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateGold(int16 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateEntiCount(int16 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateMaxHandCount(int16 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateUseHandCount(int16 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateUseChuckCount(int16 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateCurrentScore(int32 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateMaxScore(int32 _invalue)
{
	auto VM = GetVMPlayerInfo();
}

void UPlayerInfoComponent::UpdateHandRanking(const FName _Name, int Level)
{
	auto MyPlayerState = GetPlayerState();



}
