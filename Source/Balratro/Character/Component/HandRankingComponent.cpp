#include "Character/Component/HandRankingComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "Singleton/BBGameSingleton.h"




// Called when the game starts
void UHandRankingComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerState = GetPlayerState();
	auto VM = GetVMHandRanking();
	auto& Sigleton = UBBGameSingleton::Get();

    PlayerState->ResetMyHandRankingInfo(Sigleton.GetHandRankingStatTable());

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
