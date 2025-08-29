#include "Character/Component/HandRankingComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_HandRankingCount.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"

#include "Singleton/BBGameSingleton.h"

void UHandRankingComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerState = GetPlayerState();
	auto& Sigleton = UBBGameSingleton::Get();
	auto VM_PlayerInfo = GetVMPlayerInfo();
	auto VM_HandRanking = GetVMHandRanking();

    PlayerState->ResetMyHandRankingInfo(Sigleton.GetHandRankingStatTable());
	

	VM_PlayerInfo->OnClickedRunInfoButton.AddUObject(this, &UHandRankingComponent::InitHandRanking);
	VM_HandRanking->OnHandRankingExitButton.AddUObject(this, &UHandRankingComponent::ExitHandRankingView);
	VM_HandRanking->OnHandRankingBoucherButton.AddUObject(this, &UHandRankingComponent::ShowBoucherCard);



#ifdef HandRankingView_View_TEST
	InitHandRanking();
#endif
}

void UHandRankingComponent::InitHandRanking()
{
	auto PlayerState = GetPlayerState();
	auto VM = GetVMHandRanking();
	auto VM_MainMenu = GetVMMainWidget();

	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("HandRankingView", true));

	auto FHandRankingArray = PlayerState->GetHandRankingInfo();
	for (auto Info : FHandRankingArray)
	{
		VM->AddHandRankingNum(Info->_Name, Info->Info);
	}
}

void UHandRankingComponent::ExitHandRankingView()
{
	auto VM_MainMenu = GetVMMainWidget();

	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("HandRankingView", false));
}

void UHandRankingComponent::ShowBoucherCard()
{
	auto VM = GetVMHandRanking();
	auto PS = GetPlayerState();

	auto CurBouchers = PS->GetCurBoucherInfo();
	VM->SetCurHaveBouchers(CurBouchers);
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

UVM_MainMenu* UHandRankingComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}