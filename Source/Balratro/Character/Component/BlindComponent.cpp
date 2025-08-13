// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/BlindComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"

#include "UI/MVVM/ViewModel/VM_BlindSelect.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Core/MyPlayerState.h"


void UBlindComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto VM_SelectBlind = GetVMBlindSelect();
	VM_SelectBlind->OnSelectBlind.AddUObject(this, &UBlindComponent::BlindSelectEvent);

	auto VM_Store = GetVMStore();
	VM_Store->OnNextButton.AddUObject(this, &UBlindComponent::BlindViewActive);

	InitBlindSelectView();
}

void UBlindComponent::InitBlindSelectView()
{
	auto PS = GetPlayerState();
	auto VM = GetVMBlindSelect();

	auto BlindStatTable = UBBGameSingleton::Get().GetBlindStat();

	int EntiCnt = PS->GetEntiCount();

	VM->SetSmallGrade(BlindStatTable[EntiCnt]->SMallGrade);
	VM->SetBigGrade(BlindStatTable[EntiCnt]->BigGrade);
	VM->SetBossGrade(BlindStatTable[EntiCnt]->BossGrade);
}

void UBlindComponent::BlindSelectEvent(EPlayerStateType InValue)
{
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();

	if (InValue == EPlayerStateType::SMALL_BLIND || InValue == EPlayerStateType::BIG_BLIND
		|| InValue == EPlayerStateType::SMALL_BLIND)
	{
		VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("SelectBlindView",false));
		PS->SetPlayerState(InValue);
		
		auto BlindStatTable = UBBGameSingleton::Get().GetBlindStat();
		int EntiCnt = PS->GetEntiCount();
		
		if (InValue == EPlayerStateType::SMALL_BLIND)
		{
			PS->SetCurrentRoundBlindGrade(BlindStatTable[EntiCnt]->SMallGrade);
		}
		else if (InValue == EPlayerStateType::BIG_BLIND)
		{
			PS->SetCurrentRoundBlindGrade(BlindStatTable[EntiCnt]->BigGrade);
		}
		else
		{
			PS->SetCurrentRoundBlindGrade(BlindStatTable[EntiCnt]->BossGrade);
		}
		
	}
	else if(InValue == EPlayerStateType::SMALL_BLIND_SKIP || InValue == EPlayerStateType::BIG_BLIND_SKIP)
	{
		// 스킵 보상 수령하기
		return;
	}
}

void UBlindComponent::BlindViewActive()
{
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();
	
	VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("StoreView", false));
	VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("SelectBlindView", true));
}

UVM_BlindSelect* UBlindComponent::GetVMBlindSelect()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_BlindSelect");
	Context.ContextClass = UVM_BlindSelect::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_BlindSelect>(Found);
}

UVM_MainMenu* UBlindComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}

AMyPlayerState* UBlindComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}

UVM_Store* UBlindComponent::GetVMStore()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_Store");
	Context.ContextClass = UVM_Store::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_Store>(Found);
}
