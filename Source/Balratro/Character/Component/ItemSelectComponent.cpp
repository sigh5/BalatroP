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
#include "UI/MVVM/ViewModel/VM_CardDeck.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"

#include "GameData/Utills.h"

// Called when the game starts
void UItemSelectComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PS = GetPlayerState();
	auto VM_Stroe = GetVMStore();
	auto VM_CardDeck = GetVMCardDeck();
	auto VM_BlindSelect = GetVMBlindSelect();

	InitTaroInfoTable();

	PS->OnSelectNextScene.AddUObject(this, &UItemSelectComponent::SetItemSelectView);
	PS->OnBlindSkipRewardSetting.AddUObject(this, &UItemSelectComponent::SetBlindSkipReward);

	VM_CardDeck->OnSkipButtonClicked.AddUObject(this, &UItemSelectComponent::ItemSelectSceneSkip);
	VM_BlindSelect->OnSelectBlind.AddUObject(this, &UItemSelectComponent::SetBlindSkipButtonFlag);
}

void UItemSelectComponent::InitTaroInfoTable()
{
	auto Datas = UBBGameSingleton::Get().GetTaroStatTable();
	auto PS = GetPlayerState();

	TArray<class UTaroStat_Info*> TaroTable;

	for (const auto& Info : Datas)
	{
		auto NewData = NewObject<UTaroStat_Info>();
		NewData->Info = *Info;
		
		TaroTable.Add(NewData);
	}

	PS->SetTaroStatTable(TaroTable);
}

void UItemSelectComponent::ItemSelectSceneSkip()
{
	auto VM_MainMenu = GetVMMainWidget();
	auto PS = GetPlayerState();
	
	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("ItemSelectView", false));
	VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", false));

	PS->SetNextRound();

	if (IsClickedBlindSkip)
	{
		auto VM_Store = GetVMStore();
		VM_Store->NextButtonClicked();
		IsClickedBlindSkip = false;
	}
	else
	{
		PS->SetPlayerState(EPlayerStateType::STORE);
	}
}

void UItemSelectComponent::SetBlindSkipReward(EBlindSkip_Tag CurTagType)
{
	auto VM_MainMenu = GetVMMainWidget();
	VM_MainMenu->SetClearFlag(true);

	switch (CurTagType)
	{
	case EBlindSkip_Tag::SECREAT_JOKER:
		break;
	case EBlindSkip_Tag::REAR_JOKER:
		break;
	case EBlindSkip_Tag::NEGERTIVE_JOKER:
		break;
	case EBlindSkip_Tag::FOIL_JOKER:
		break;
	case EBlindSkip_Tag::BOSTER_PACK_FREE:
		break;
	case EBlindSkip_Tag::ADD_TEN_JKER:
		break;
	case EBlindSkip_Tag::MULTIPLE_JOKER:
		break;
	case EBlindSkip_Tag::BOSS_GOLD_REWARD:
		break;
	case EBlindSkip_Tag::VOUCHER_ADD:
		break;
	case EBlindSkip_Tag::TWO_COMMON_JOKER:
		break;
	case EBlindSkip_Tag::JUGGLE:
		break;
	case EBlindSkip_Tag::BOSS_REROAD:
		break;

	case EBlindSkip_Tag::STANDARD_PACK: 
	case EBlindSkip_Tag::ARCANA_PACK:
	case EBlindSkip_Tag::ORB_PACK:
	case EBlindSkip_Tag::GHOST_PACK:
		BlindSkipReward_BoosterPackSetting(CurTagType);
		break;
	default:
		break;
	}

}

void UItemSelectComponent::BlindSkipReward_BoosterPackSetting(EBlindSkip_Tag CurTagType)
{
	auto PS = GetPlayerState();

	EBoosterPackType ItemType = EBoosterPackType::TARO_MEGA;
	UBoosterPackData* CurPack = NewObject<UBoosterPackData>();

	CurPack->SetType(ItemType);

	PS->SetSelectPackType(CurPack);
}

void UItemSelectComponent::SetBlindSkipButtonFlag(EPlayerStateType InType)
{
	if (InType == EPlayerStateType::BIG_BLIND_SKIP || InType == EPlayerStateType::SMALL_BLIND_SKIP)
	{
		IsClickedBlindSkip = true;
	}
}

void UItemSelectComponent::SetItemSelectView(EPlayerStateType _InType)
{
	if (_InType == EPlayerStateType::ITEM_SELECT)
	{
		auto VM_MainMenu = GetVMMainWidget();
		VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("ItemSelectView", true));

		SetItemList();
	}
	else if (_InType == EPlayerStateType::RESET_GAME)
	{
		InitTaroInfoTable();
		IsClickedBlindSkip = false;
	}
}

void UItemSelectComponent::SetItemList()
{
	auto PS = GetPlayerState();
	auto VM_ItemSelectView = GetVMItemSelect();
	auto PackType = PS->GetSelectPackType();
	
	CurShowTaroInfo.Empty();
	//// PackType == > Taro / Ghost / Orb / CardPack 에 따라 다르게 세팅

	int32 IsMegaNum = (static_cast<int32>(PackType->GetType()) % 2 == 0 ) ? 5 : 3 ;

	// 메가 5장, 노말 3장 리스트에 올려야됌
	// 메가면 2장 선택 아니면 1장 선택
	if (IsMegaNum == 5)
	{
		PS->SetCurSelectTaroNum(2);
	}
	else
	{
		PS->SetCurSelectTaroNum(1);
	}

	TSet<int32> IndexList = SetTaroType(IsMegaNum);
	for (UTaroStat_Info* Info : PS->GetTaroStatTable())
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

TSet<int32> UItemSelectComponent::SetTaroType(int32 SetTaroNum)
{
	auto PS = GetPlayerState();

	TSet<int32> IndexArr;

	while (IndexArr.Num() < SetTaroNum)
	{
		int32 TotalWeight = 0;
		for (auto& Elem : PS->GetTaroStatTable())
			TotalWeight += Elem->Info.weight;

		int32 RandomValue = FRandomUtils::RandomSeed.RandRange(1, TotalWeight);
		int32 Accumulated = 0;

		for (auto& Elem : PS->GetTaroStatTable())
		{
			Accumulated += Elem->Info.weight;
			if (RandomValue <= Accumulated)
			{
				IndexArr.Add(Elem->Info.index);
				break;
			}
		}
	}

	// 미구현 목록 0,2,4,9,11,12,13   
	// 조커 카드 구현 하고 해야되는것 10,14,20,22

	// 스톤카드 수정필요 
	//TSet<int32> IndexArr = { 16,4,9,11,12 };

	
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

UVM_Store* UItemSelectComponent::GetVMStore()
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

UVM_CardDeck* UItemSelectComponent::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}

UVM_BlindSelect* UItemSelectComponent::GetVMBlindSelect()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_BlindSelect");
	Context.ContextClass = UVM_BlindSelect::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_BlindSelect>(Found);
}
