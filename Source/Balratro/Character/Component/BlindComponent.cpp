// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/BlindComponent.h"

#include "Kismet/GameplayStatics.h"
#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"

#include "UI/MVVM/ViewModel/VM_BlindSelect.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

#include "Core/MyPlayerState.h"


void UBlindComponent::BeginPlay()
{
	Super::BeginPlay();

	auto VM_SelectBlind = GetVMBlindSelect();
	VM_SelectBlind->OnSelectBlind.AddUObject(this, &UBlindComponent::BlindSelectEvent);

	auto VM_Store = GetVMStore();
	VM_Store->OnNextButton.AddUObject(this, &UBlindComponent::BlindViewActive);

	auto PS = GetPlayerState();
	PS->OnRestCardsSet.AddUObject(this,&UBlindComponent::UseBossSkill);

	ResetBlindSelectData();
}

void UBlindComponent::InitBlindSelectView()
{
	auto PS = GetPlayerState();
	auto VM = GetVMBlindSelect();
	auto BlindStatTable = UBBGameSingleton::Get().GetBlindStat();

	int32 EntiCnt = PS->GetEntiCount();

	VM->SetSmallGrade(BlindStatTable[EntiCnt]->SMallGrade);
	VM->SetBigGrade(BlindStatTable[EntiCnt]->BigGrade);
	VM->SetBossGrade(BlindStatTable[EntiCnt]->BossGrade);

	auto& BossType = PS->GetCurBossType();

	if (BossType.Key != EntiCnt)
	{
		SetRandomBossType();
	}

	

	VM->SetBossType(BossType.Value);
}

void UBlindComponent::BlindSelectEvent(EPlayerStateType InValue)
{
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();
	auto VM_BlindSelect = GetVMBlindSelect();

	if (InValue == EPlayerStateType::SMALL_BLIND || InValue == EPlayerStateType::BIG_BLIND
		|| InValue == EPlayerStateType::BOSS_BLIND)
	{
		VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("SelectBlindView", false));
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
	else if (InValue == EPlayerStateType::SMALL_BLIND_SKIP || InValue == EPlayerStateType::BIG_BLIND_SKIP)
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

	PS->SetPlayerState(EPlayerStateType::NONE);

	InitBlindSelectView();
}

void UBlindComponent::ResetBlindSelectData()
{
	auto PS = GetPlayerState();
	
	BossTypes.Add(EBossType::HOOK, [this]() { HOOK_Skill(); });
	BossTypes.Add(EBossType::OX, [this]() { OX_Skill(); });
	BossTypes.Add(EBossType::WALL, [this]() { WALL_SKill(); });
	BossTypes.Add(EBossType::ARM, [this]() { ARM_Skill(); });
	BossTypes.Add(EBossType::PSYCHIC, [this]() { PSYCHIC_Skill(); });
	BossTypes.Add(EBossType::GOAD, [this]() { GOAD_Skill(); });
	BossTypes.Add(EBossType::WATER, [this]() { WATER_Skill(); });
	BossTypes.Add(EBossType::EYE, [this]() { EYE_Skill(); });

	for (int32 i = 1; i <= 8; i++)
	{
		RandomArray.Add(i);
	}

	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(nullptr);
	int32 Seed = FMath::FloorToInt(CurrentTime * 1000.0f);
	FMath::RandInit(Seed);

	int32 NumElements = RandomArray.Num();

	for (int32 i = NumElements - 1; i > 0; --i)
	{
		int32 RandomIndex = FMath::RandRange(0, i);

		if (i != RandomIndex)
		{
			RandomArray.Swap(i, RandomIndex);
		}
	}
	
	PS->SetCurBossType({ -1,EBossType::NONE });
	InitBlindSelectView();
}

void UBlindComponent::SetRandomBossType()
{
	auto PS = GetPlayerState();
	
	TPair<int32, EBossType> MyBossType;
	
	/*int32 EntiCount = PS->GetEntiCount();
	MyBossType = { EntiCount ,static_cast<EBossType>(RandomArray[EntiCount])};
	
	RandomArray.RemoveAt(EntiCount);*/

	// TestCode
	MyBossType = { 0,EBossType::HOOK };

	PS->SetCurBossType(MyBossType);
}

void UBlindComponent::UseBossSkill()
{
	auto PS = GetPlayerState();
	
	if (EPlayerStateType::BOSS_BLIND != PS->GetPlayerState())
	{
		return;
	}

	EBossType CurBossType = PS->GetCurBossType().Value;
	BossTypes[CurBossType]();
}

void UBlindComponent::HOOK_Skill()
{
	auto PS = GetPlayerState();
	auto VM_CardDeck = GetVMCardDeck();

	auto RestHands = PS->GetRestCardInHands();
	
	if (RestHands.Num() == 0)
		return;

	if (RestHands.Num() <= 2)
	{
		RestHands.Empty();
	}
	else
	{
		RestHands.RemoveAt(0, 2);
	}

	PS->SetRestCardInHands(RestHands); // 남아있는 
}

void UBlindComponent::OX_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("OX_Skill"));
}

void UBlindComponent::WALL_SKill()
{
	auto PS = GetPlayerState();

	UE_LOG(LogTemp, Warning, TEXT("WALL_SKill"));
}

void UBlindComponent::ARM_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("ARM_Skill"));
}

void UBlindComponent::PSYCHIC_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("PSYCHIC_Skill"));
}

void UBlindComponent::GOAD_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("GOAD_Skill"));
}

void UBlindComponent::WATER_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("WATER_Skill"));
}

void UBlindComponent::EYE_Skill()
{
	UE_LOG(LogTemp, Warning, TEXT("EYE_Skill"));
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

UVM_CardDeck* UBlindComponent::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}