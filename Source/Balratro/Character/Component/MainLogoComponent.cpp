#include "Character/Component/MainLogoComponent.h"

#include "Kismet/GameplayStatics.h"
#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_MainMenu.h"

void UMainLogoComponent::BeginPlay()
{
	Super::BeginPlay();

	auto VM = GetVMMainWidget();
	auto PS = GetPlayerState();

	VM->OnMainPlayButtonClicked.AddUObject(this, &UMainLogoComponent::BlindSelctViewStart);
	PS->OnSelectNextScene.AddUObject(this, &UMainLogoComponent::InitLogoView);
}

void UMainLogoComponent::BlindSelctViewStart()
{
	auto PS = GetPlayerState();
	auto VM = GetVMMainWidget();

	PS->SetPlayerState(EPlayerStateType::BLINDSELECT);

	VM->SetCurWidgetName(FWidgetFlag_Info("PlayerInfoView", true));
	VM->SetCurWidgetName(FWidgetFlag_Info("JokerSlotView", true));
}

void UMainLogoComponent::InitLogoView(EPlayerStateType _InValue)
{
	if (_InValue != EPlayerStateType::LOGO)
		return;



	auto VM = GetVMMainWidget();
	VM->SetMainLogoFlag(true);
}

UVM_MainMenu* UMainLogoComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}

AMyPlayerState* UMainLogoComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}

