#include "Character/Component/RewardComponent.h"

#include "Core/MyPlayerState.h"
#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_Reward.h"

#include "Singleton/BBGameSingleton.h"

void URewardComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto PS = GetPlayerState();
	auto VM_Reward = GetVMReward();

	PS->OnSelectNextScene.AddUObject(this, &URewardComponent::SetRewardViewData);
	PS->OnSelectNextScene.AddUObject(this, &URewardComponent::AddEarnGold);

	VM_Reward->OnCashButtonClicked.AddUObject(this, &URewardComponent::StartStoreView);
}

void URewardComponent::SetRewardViewData(EPlayerStateType InType)
{
	if (EPlayerStateType::REWARD != InType)
	{
		return;
	}
	
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();
	auto VM_PI = GetVMPlayerInfo();
	auto VM_Reward = GetVMReward();

	VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("RewardView", true));

	VM_Reward->SetRestHands(0);
	VM_Reward->SetBlindReward(0);
	VM_Reward->SetInterest(0);


	int32 RestHands = PS->GetMaxHandCount() - PS->GetUseHandCount();
	int32 RestChucks = PS->GetMaxChuckCount() - PS->GetUseChuckCount();
	int32 CurGold = PS->GetGold();
	int32 BlindGrade = VM_PI->GetBlindGrade();
	int32 BlindReward = VM_PI->GetBlindReward();
	int32 BlindImageIndex = VM_PI->GetBlindImageIndex();
	UE_LOG(LogTemp, Warning, TEXT("!! %d"), BlindImageIndex);

	VM_Reward->SetRestHands(RestHands);
	VM_Reward->SetBlindReward(BlindReward);
	VM_Reward->SetInterest(CurGold / 5);
	VM_Reward->SetBlindGrade(BlindGrade);
	VM_Reward->SetBlindImageIndex(BlindImageIndex);

	EarnGold = RestHands + BlindReward + (CurGold / 5);

	VM_Reward->SetEarnGold(EarnGold);


	PS->SetNextRound();
}

void URewardComponent::StartStoreView()
{
	auto PS = GetPlayerState();
	auto VM_MainWidget = GetVMMainWidget();

	PS->SetPlayerState(EPlayerStateType::STORE);
	VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("RewardView", false));
}

void URewardComponent::AddEarnGold(EPlayerStateType _InType)
{
	auto PS = GetPlayerState();

	int32 Gold = PS->GetGold();
	PS->SetGold(Gold + EarnGold);
	EarnGold = 0;
}

UVM_MainMenu* URewardComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}


AMyPlayerState* URewardComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}


UVM_PlayerInfo* URewardComponent::GetVMPlayerInfo()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_PlayerInfo");
	Context.ContextClass = UVM_PlayerInfo::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_PlayerInfo>(Found);
}

UVM_Reward* URewardComponent::GetVMReward()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_Reward");
	Context.ContextClass = UVM_Reward::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_Reward>(Found);
}
