// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/BlindComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"

#include "UI/MVVM/ViewModel/VM_BlindSelect.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"

#include "Core/MyPlayerState.h"


void UBlindComponent::BeginPlay()
{
	Super::BeginPlay();
	
	auto VM_SelectBlind = GetVMBlindSelect();
	VM_SelectBlind->OnSelectBlind.AddUObject(this, &UBlindComponent::BlindSelectEvent);

	InitBlindSelectView();
}

void UBlindComponent::InitBlindSelectView()
{
	auto PS = GetPlayerState();
	auto VM = GetVMBlindSelect();

	auto BlindStatTable = UBBGameSingleton::Get().GetBlindStat();

	int RoundCnt = PS->GetRoundCount();

	VM->SetSmallGrade(BlindStatTable[RoundCnt]->SMallGrade);
	VM->SetBigGrade(BlindStatTable[RoundCnt]->BigGrade);
	VM->SetBossGrade(BlindStatTable[RoundCnt]->BossGrade);
}

void UBlindComponent::BlindSelectEvent(EPlayerStateType InValue)
{
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();

	if (InValue == EPlayerStateType::SMALL_BLIND || InValue == EPlayerStateType::BIG_BLIND
		|| InValue == EPlayerStateType::SMALL_BLIND)
	{
		FName CurName = "SelectBlindView";
		VM_MainWidget->SetCurWidgetName(CurName);
		PS->SetPlayerState(InValue);
	}
	else if(InValue == EPlayerStateType::SMALL_BLIND_SKIP || InValue == EPlayerStateType::BIG_BLIND_SKIP)
	{
		// 스킵 보상 수령하기
		return;
	}
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