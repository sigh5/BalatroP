#include "Character/Component/HandRankingComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

#include "Singleton/BBGameSingleton.h"

void UHandRankingComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerState = GetPlayerState();
	auto& Sigleton = UBBGameSingleton::Get();

    PlayerState->ResetMyHandRankingInfo(Sigleton.GetHandRankingStatTable());


	auto VM_PlayerInfo = GetVMPlayerInfo();
	VM_PlayerInfo->OnClickedRunInfoButton.AddUObject(this, &UHandRankingComponent::InitHandRanking);

#ifdef HandRankingView_View_TEST
	InitHandRanking();
#endif
}

void UHandRankingComponent::InitHandRanking()
{
	auto PlayerState = GetPlayerState();
	auto VM = GetVMHandRanking();


	auto FHandRankingArray = PlayerState->GetHandRankingInfo();
	for (auto Info : FHandRankingArray)
	{
		VM->AddHandRankingNum(Info->_Name, Info->Info);
	}
}

UVM_HandRankingCount* UHandRankingComponent::GetVMHandRanking()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_HandRankingCount");
	Context.ContextClass = UVM_HandRankingCount::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_HandRankingCount>(Found);
}

AMyPlayerState* UHandRankingComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;	
}

UVM_PlayerInfo* UHandRankingComponent::GetVMPlayerInfo()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_PlayerInfo");
	Context.ContextClass = UVM_PlayerInfo::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_PlayerInfo>(Found);
}