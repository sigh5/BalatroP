#include "Character/Component/StoreComponent.h"

#include "Core/MyPlayerState.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"


void UStoreComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PS = GetPlayerState();

	PS->OnSelectNextScene.AddUObject(this, &UStoreComponent::SetStoreView);
}

void UStoreComponent::SetStoreView(EPlayerStateType _InType)
{
	if (_InType != EPlayerStateType::STORE)
		return;

	auto PS = GetPlayerState();
	auto VM_MainMenu = GetVMMainWidget();

	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("StoreView", true));

}

UVM_MainMenu* UStoreComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}


AMyPlayerState* UStoreComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}
