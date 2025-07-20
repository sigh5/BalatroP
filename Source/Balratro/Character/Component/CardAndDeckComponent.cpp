#include "Character/Component/CardAndDeckComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

// 142 /190

void UCardAndDeckComponent::UpdateCardInHand(int32 _invalue)
{
	auto VM = GetVMCardDeck();
}

void UCardAndDeckComponent::UpdateCardInDeck(int32 _invalue)
{
	auto VM = GetVMCardDeck();
	auto PS = GetPlayerState();
	
	VM->SetDeckNum(PS->GetCardInDeck());
}

void UCardAndDeckComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateCardInDeck(52);
}

UVM_CardDeck* UCardAndDeckComponent::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}

AMyPlayerState* UCardAndDeckComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}



