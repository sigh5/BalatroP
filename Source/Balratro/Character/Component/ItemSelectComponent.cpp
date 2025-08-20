#include "Character/Component/ItemSelectComponent.h"

#include "Core/MyPlayerState.h"
#include "Item/BoosterPackData.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "PaperSprite.h"

#include "Singleton/BBGameSingleton.h"

#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"
#include "UI/MVVM/ViewModel/VM_ItemSelect.h"

// Called when the game starts
void UItemSelectComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PS = GetPlayerState();
	auto VM_Stroe = GetVMPStore();
	
	InitTaroInfoTable();

	PS->OnSelectNextScene.AddUObject(this, &UItemSelectComponent::SetItemSelectView);
}

void UItemSelectComponent::InitTaroInfoTable()
{
	auto Datas = UBBGameSingleton::Get().GetTaroStatTable();

	for (const auto& Info : Datas)
	{
		auto NewData = NewObject<UTaroStat_Info>();
		NewData->Info.index = Info->index;
		NewData->Info.weight = Info->weight;
		NewData->Info.useNum = Info->useNum;
		NewData->Info.SpriteAsset = Info->SpriteAsset;

		CurTaroStatTable.Add(NewData);
	}
}

void UItemSelectComponent::SetItemSelectView(EPlayerStateType _InType)
{
	if (_InType != EPlayerStateType::ITEM_SELECT)
		return;

	auto VM_MainMenu = GetVMMainWidget();
	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("ItemSelectView", true));

	SetItemList();
}

void UItemSelectComponent::SetItemList()
{
	auto PS = GetPlayerState();
	auto VM_ItemSelectView = GetVMItemSelect();
	auto PackType = PS->GetSelectPackType();
	
	CurShowTaroInfo.Empty();
	//// PackType == > Taro / Ghost / Orb / CardPack 에 따라 다르게 세팅

	bool IsMega = (static_cast<int32>(PackType->GetType()) % 2) ? true : false ;
	// 5장은 리스트에 올려야됌
	// 메가면 2장 선택 아니면 1장 선택
	
	TSet<int32> IndexList = SetTaroType();
	for (UTaroStat_Info* Info : CurTaroStatTable)
	{
		if (Info && IndexList.Contains(Info->Info.index))
		{
			if (!Info->Info.SpriteAsset.IsValid())
			{
				Info->Info.SpriteAsset.LoadSynchronous();
			}

			CurShowTaroInfo.Add(Info->Info);
		}
	}

	VM_ItemSelectView->SetShowTaroInfo(CurShowTaroInfo);
}

TSet<int32> UItemSelectComponent::SetTaroType()
{
	TSet<int32> IndexArr;

	while (IndexArr.Num() < 5)
	{
		int32 TotalWeight = 0;
		for (auto& Elem : CurTaroStatTable)
			TotalWeight += Elem->Info.weight;

		int32 RandomValue = FMath::RandRange(1, TotalWeight);
		int32 Accumulated = 0;

		for (auto& Elem : CurTaroStatTable)
		{
			Accumulated += Elem->Info.weight;
			if (RandomValue <= Accumulated)
			{
				IndexArr.Add(Elem->Info.index);
				break;
			}
		}
	}
	
	return IndexArr;
}

UVM_MainMenu* UItemSelectComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}

UVM_ItemSelect* UItemSelectComponent::GetVMItemSelect()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_ItemSelect");
	Context.ContextClass = UVM_ItemSelect::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_ItemSelect>(Found);
}

UVM_Store* UItemSelectComponent::GetVMPStore()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_Store");
	Context.ContextClass = UVM_Store::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_Store>(Found);
}

AMyPlayerState* UItemSelectComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}
