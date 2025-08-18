#include "Character/Component/StoreComponent.h"

#include "Core/MyPlayerState.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Item/BoosterPackData.h"


void UStoreComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemWeights = {
   { EBoosterPackType::TARO_BASE, 5 },
   { EBoosterPackType::TARO_MEGA, 3 },
   /*{ EBoosterPackType::CARD_BASE, 5 },
   { EBoosterPackType::CARD_MEGA, 3 },
   { EBoosterPackType::ORB_BASE, 5 },
   { EBoosterPackType::ORB_MEGA, 3 },
   { EBoosterPackType::JOKER_BASE, 5 },
   { EBoosterPackType::JOKER_MEGA, 3 },
   { EBoosterPackType::GHOST_BASE, 2 },
   { EBoosterPackType::GHOST_MEGA, 1 }*/ };


	auto PS = GetPlayerState();
	auto VM = GetVMPStore();

	PS->OnSelectNextScene.AddUObject(this, &UStoreComponent::SetStoreView);
	VM->OnReRollButton.AddUObject(this, &UStoreComponent::ReRollCostUp);
}

void UStoreComponent::SetStoreView(EPlayerStateType _InType)
{
	if (_InType != EPlayerStateType::STORE)
		return;

	auto PS = GetPlayerState();
	auto VM_MainMenu = GetVMMainWidget();

	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("StoreView", true));

	SetUpStoreItem();
	SetBoucherItem();
	SetDownStoreItem();
}

void UStoreComponent::ReRollCostUp()
{
	auto PS = GetPlayerState();

	int32 CurCost = PS->GetRerollCost();
	PS->SetRerollCost(CurCost + 2);
}

void UStoreComponent::SetUpStoreItem()
{

}

void UStoreComponent::SetBoucherItem()
{

}

void UStoreComponent::SetDownStoreItem()
{
	auto PS = GetPlayerState();
	auto VM_Stroe = GetVMPStore();

	BoosterPacks.Empty();

	TArray<EBoosterPackType> BoosterPackIndexs;
	int32 BoosterPackNum = 2; // PS->GetDownStoreNum();

	for (int i = 0; i < BoosterPackNum; ++i)
	{
		EBoosterPackType ItemType = SetItemType();
		BoosterPackIndexs.Add(ItemType); // View를 위한 데이타

		UBoosterPackData* CurPack = NewObject<UBoosterPackData>();
		CurPack->SetType(ItemType);
		BoosterPacks.Add(CurPack);  // 컴포넌트에서 사용할 데이타
	}

	VM_Stroe->SetBoosterPackIndexs(BoosterPackIndexs);
}

EBoosterPackType UStoreComponent::SetItemType()
{
	int32 TotalWeight = 0;
	for (auto& Elem : ItemWeights)
		TotalWeight += Elem.Weight;

	int32 RandomValue = FMath::RandRange(1, TotalWeight);
	int32 Accumulated = 0;

	for (auto& Elem : ItemWeights)
	{
		Accumulated += Elem.Weight;
		if (RandomValue <= Accumulated)
			return Elem.Type;
	}

	return EBoosterPackType::NONE;
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


UVM_Store* UStoreComponent::GetVMPStore()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_Store");
	Context.ContextClass = UVM_Store::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_Store>(Found);
}

AMyPlayerState* UStoreComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}
