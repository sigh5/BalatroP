// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/JokerCardComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

// 디버그용
#include "PaperSprite.h"

void UJokerCardComponent::BeginPlay()
{
	Super::BeginPlay();

	auto VM_Joker = GetVMJockerSlot();
	auto PS = GetPlayerState();

	VM_Joker->OnAddJoker.AddUObject(this, &UJokerCardComponent::UpdateAddJoker);


#ifdef JokerSlotView_VIEW_TEST
	
	FJokerStat Test_Data;
	EJokerType ItemType = EJokerType::BASE_JOKER;
	
	int32 ItemIndex = 0;
	FString ItemIndexStr = FString::FromInt(ItemIndex);
	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Jokers_Sprite_%s.Jokers_Sprite_%s"), *ItemIndexStr, *ItemIndexStr);
	Test_Data.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Test_Data.CardSprite.IsValid())
	{
		Test_Data.CardSprite.LoadSynchronous();
	}

	Test_Data.JokerType = ItemType;
	Test_Data.Price = 3;
	UpdateAddJoker(Test_Data);

#endif
}

void UJokerCardComponent::UpdateJoker()
{
	

}

void UJokerCardComponent::CalculatorJoker(UJokerCard_Info* JokerCard)
{
	auto PS = GetPlayerState();
	auto VM_PlayerInfo = GetVMPlayerInfo();

	int32 CurrentChip = PS->GetCurrentShowChip();
	int32 CurrentDrainage = PS->GetCurrentShowDrainage();

	EJokerType Type = EJokerType::BASE_JOKER;		//(JokerType)(JokerCard->Info.Stat);
	if (Type == EJokerType::BASE_JOKER)
	{
		VM_PlayerInfo->SetCurDrainage(CurrentDrainage * 4);
	}


}

void UJokerCardComponent::UpdateAddJoker(FJokerStat& Data)
{
	auto VM_Joker = GetVMJockerSlot();
	auto PS = GetPlayerState();
	PS->AddCurrentJokerCard(Data);

	auto& JokerCards = PS->GetCurrentJokerCards();
	
	VM_Joker->SetJokerDatas(JokerCards);
}

UVM_JockerSlot* UJokerCardComponent::GetVMJockerSlot()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_JockerSlot");
	Context.ContextClass = UVM_JockerSlot::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_JockerSlot>(Found);
}

UVM_PlayerInfo* UJokerCardComponent::GetVMPlayerInfo()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_PlayerInfo");
	Context.ContextClass = UVM_PlayerInfo::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_PlayerInfo>(Found);
}

AMyPlayerState* UJokerCardComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}
