#include "Character/Component/GameOverComponent.h"

#include "UI/MVVM/ViewModel/VM_GameOver.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "PaperSprite.h"

#include "Core/MyPlayerState.h"

#include "GameData/Utills.h"

void UGameOverComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PS = GetPlayerState();
	auto VM = GetVMGameOver();

	PS->OnSelectNextScene.AddUObject(this, &UGameOverComponent::SetGameOverView);

	VM->OnNewRunButtonEvent.AddUObject(this, &UGameOverComponent::SetNewRunEvent);
}

void UGameOverComponent::SetGameOverView(EPlayerStateType InType)
{
	if (InType != EPlayerStateType::GAME_OVER)
		return;

	auto VM_GameOver = GetVMGameOver();
	auto VM_MainMenu = GetVMMainWidget();
	auto PS = GetPlayerState();

	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("GameOverView", true));

	TArray<UHandRanking_Info*> HandRankingInfo = PS->GetHandRankingInfo();
	HandRankingInfo.Sort([&](const UHandRanking_Info& a, const UHandRanking_Info& b)
		{
			return a.Info.UseNum > b.Info.UseNum;
		});

	
	FGameOverInfo Infos;

	Infos.BestHandScore =  PS->GetMaxScore();
	Infos.PlayCount = PS->GetAllPlayCount();
	Infos.ChunkCount = PS->GetAllChuckCount();
	Infos.EntiCount = PS->GetEntiCount() + 1;
	Infos.RoundCount = PS->GetRoundCount();

	auto Seed = FRandomUtils::RandomSeed;
	
	int32 SeedNum = Seed.GetCurrentSeed();

	FName MostHandRankingName = HandRankingInfo[0]->_Name;

	VM_GameOver->SetSeedName(SeedNum);
	VM_GameOver->SetMostHandRankingName(MostHandRankingName);
	VM_GameOver->SetGameOver_Infos(Infos);

}

void UGameOverComponent::SetNewRunEvent()
{
	auto PS = GetPlayerState();
	PS->ResetInfos();
	
	auto VM_MainMenu = GetVMMainWidget();

	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("GameOverView", false));
	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", false));

	// 나중에 LOGO로 바꾸면서 아래쪽에 있는 코드도 수정 필요
	PS->SetPlayerState(EPlayerStateType::BLINDSELECT); 
	auto VM = GetVMBlindSelect();
	VM->SetResetBlindView(true);
}


UVM_MainMenu* UGameOverComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}

UVM_GameOver* UGameOverComponent::GetVMGameOver()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_GameOver");
	Context.ContextClass = UVM_GameOver::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_GameOver>(Found);
}

UVM_Store* UGameOverComponent::GetVMStore()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_Store");
	Context.ContextClass = UVM_Store::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_Store>(Found);
}

AMyPlayerState* UGameOverComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}

UVM_BlindSelect* UGameOverComponent::GetVMBlindSelect()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_BlindSelect");
	Context.ContextClass = UVM_BlindSelect::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_BlindSelect>(Found);
}