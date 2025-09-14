#include "Character/Component/StoreComponent.h"

#include "Core/MyPlayerState.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>
#include "PaperSprite.h"

#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Item/BoosterPackData.h"

#include "Engine/AssetManager.h"
#include "GameData/Utills.h"

#include "Singleton/BBGameSingleton.h"

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
	auto VM = GetVMStore();

	PS->OnSelectNextScene.AddUObject(this, &UStoreComponent::SetStoreView);
	VM->OnReRollButton.AddUObject(this, &UStoreComponent::ReRollCostUp);
	VM->OnBuyBoosterPack.AddUObject(this, &UStoreComponent::StartBoosterPackEvent);
	VM->OnBuyBoucherCard.AddUObject(this, &UStoreComponent::EraseStoreBoucherCard);

	InitStoreData();

#ifdef Store_View_TEST
	SetDownStoreItem();
	SetUpStoreItem();
	SetBoucherItem();
#endif
}

void UStoreComponent::SetStoreView(EPlayerStateType _InType)
{
	if (_InType == EPlayerStateType::RESET_GAME)
	{
		InitStoreData();
		return;
	}

	auto PS = GetPlayerState();
	auto VM_MainMenu = GetVMMainWidget();

	if (_InType == EPlayerStateType::ITEM_SELECT)
	{
		VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("StoreView", false));
		return;
	}
	else if (_InType != EPlayerStateType::STORE)
		return;

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
	auto PS = GetPlayerState();
	auto VM_Stroe = GetVMStore();

	if (JokerCards.Num() > 0)
	{
		for (auto Joker : JokerCards)
		{
			Available_JokerInfos.Add(Joker->Info.JokerType);
		}
	}

	JokerCards.Empty();

	float HavePriceDownBoucher = PS->GetIsHavePriceDownBoucher() ? 0.65f : 1.f;


	int32 UpStoreItemNum = PS->GetHaveUpStoreNum();
	auto OriginJokerTable = UBBGameSingleton::Get().GetJokerStatTable();

	for (int i = 0; i < UpStoreItemNum; ++i)
	{
		EJokerType ItemType = SetJokerType();

		if (ItemType == EJokerType::NONE)
			return;

		UJokerCard_Info* CurJoker = NewObject<UJokerCard_Info>();

		int32 ItemIndex = static_cast<int32>(ItemType);
		CurJoker->Info.Name = OriginJokerTable[ItemIndex]->Name;
		CurJoker->Info.JokerType = OriginJokerTable[ItemIndex]->JokerType;
		CurJoker->Info.JokerGrade = OriginJokerTable[ItemIndex]->JokerGrade;
		CurJoker->Info.JokerSFX = OriginJokerTable[ItemIndex]->JokerSFX;
		CurJoker->Info.Price = FMath::FloorToInt(OriginJokerTable[ItemIndex]->Price * HavePriceDownBoucher);
		CurJoker->Info.UseNum = OriginJokerTable[ItemIndex]->UseNum;
		CurJoker->Info.CardSprite = OriginJokerTable[ItemIndex]->CardSprite;

		JokerCards.Add(CurJoker);
	}

	VM_Stroe->SetStoreJokerData(JokerCards);
}

void UStoreComponent::SetBoucherItem()
{
	auto VM_Store = GetVMStore();

	if (BoucherCards.Num() == 0)
	{
		SetBoucherType();	
		VM_Store->SetCurStoreBouchers(BoucherCards);
	}
}

void UStoreComponent::SetDownStoreItem()
{
	auto PS = GetPlayerState();
	auto VM_Stroe = GetVMStore();

	int32 BoosterPackNum = PS->GetHaveBoosterPackNum();
	int32 MaxBoosterPackNum = PS->GetMaxHaveBoosterPackNum();

	if (BoosterPackNum == MaxBoosterPackNum)
	{
		BoosterPacks.Empty();
		for (int i = 0; i < BoosterPackNum; ++i)
		{
			EBoosterPackType ItemType = SetBoosterPackType();
			UBoosterPackData* CurPack = NewObject<UBoosterPackData>();

			int32 ItemIndex = static_cast<int32>(ItemType);
			FString ItemIndexStr = FString::FromInt(ItemIndex);
			FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Booster/boosters_Sprite_%s.boosters_Sprite_%s"), *ItemIndexStr, *ItemIndexStr);
			CurPack->PackMesh = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
			if (!CurPack->PackMesh.IsValid())
			{
				CurPack->PackMesh.LoadSynchronous();
			}

			CurPack->SetType(ItemType);
			CurPack->SetIndex(BoosterPackIndex++);
			BoosterPacks.Add(CurPack);
		}
	}
	
	VM_Stroe->SetBoosterPackTypes(BoosterPacks);
}

void UStoreComponent::EraseStoreBoucherCard(FBoucherInfo& _Info)
{
	auto PS = GetPlayerState();

	int32 CurPrice = _Info.Price;

	if (BoucherCards.Contains(_Info))
	{
		BoucherCards.RemoveSingle(_Info);
	}

	int32 CurGold = PS->GetGold();
	PS->SetGold(CurGold - CurPrice);

	PS->AddBoucherType(_Info);
}

void UStoreComponent::InitStoreData()
{
	Available_BoucherInfos.Empty();
	BoosterPackIndex = 0;
	JokerCards.Empty();
	BoosterPacks.Empty();

	FPrimaryAssetId AssetId(TEXT("BoucherStat"), TEXT("DT_BoucherStat"));
	FAssetData AssetData;

	if (UAssetManager::Get().GetPrimaryAssetData(AssetId, AssetData))
	{
		UBoucherStat* NewStat = Cast<UBoucherStat>(AssetData.GetAsset());

		for (const auto& Boucher : NewStat->BoucherInfos)
		{
			FBoucherInfo BoucherInfo = Boucher;
			Available_BoucherInfos.Add(BoucherInfo);
		}
	}
	ensure(Available_BoucherInfos.Num() > 0);

	auto OriginJokerDatas = UBBGameSingleton::Get().GetJokerStatTable();
	
	for (auto Data : OriginJokerDatas)
	{
		Available_JokerInfos.Add(Data->JokerType);
	}
}

EBoosterPackType UStoreComponent::SetBoosterPackType()
{
	int32 TotalWeight = 0;
	for (auto& Elem : ItemWeights)
		TotalWeight += Elem.Weight;

	int32 RandomValue = FRandomUtils::RandomSeed.RandRange(1, TotalWeight);
	int32 Accumulated = 0;

	for (auto& Elem : ItemWeights)
	{
		Accumulated += Elem.Weight;
		if (RandomValue <= Accumulated)
			return Elem.Type;
	}

	return EBoosterPackType::ORB_MEGA;
}

EJokerType UStoreComponent::SetJokerType()
{
	EJokerType CurType = EJokerType::NONE;

	int32 Num = Available_JokerInfos.Num();

	int32 RandomValue = FRandomUtils::RandomSeed.RandRange(0, Num - 1);

	CurType = Available_JokerInfos[RandomValue];

	Available_JokerInfos.RemoveAtSwap(RandomValue);

	return CurType;
}

void UStoreComponent::SetBoucherType()
{
	int32 Num = Available_BoucherInfos.Num();

	int32 RandomValue = FRandomUtils::RandomSeed.RandRange(0, Num - 1);
		
	auto Data = Available_BoucherInfos[RandomValue];

	if (Data.Type == EBoucherType::BoucherType_INVENTORY_ORB)
	{
		auto PS = GetPlayerState();

		bool bIsHavePreBoucher = false;

		for (auto HaveBoucher : PS->GetCurBoucherInfo())
		{
			if (HaveBoucher.Type == Data.PrevType)
			{
				bIsHavePreBoucher = true;
				break;
			}
		}

		if (!bIsHavePreBoucher)
		{
			SetBoucherType();
		}
	}


	BoucherCards.Add(Data);
}

void UStoreComponent::StartBoosterPackEvent(UBoosterPackData* InData)
{
	auto VM_Stroe = GetVMStore();
	auto VM_MainMenu = GetVMMainWidget();
	auto PS = GetPlayerState();
	UBoosterPackData* CurBoosterPack = nullptr;

	for (int32 i = 0; i < BoosterPacks.Num(); i++)
	{
		if (BoosterPacks[i]->GetIndex() == InData->GetIndex())
		{
			CurBoosterPack = BoosterPacks[i];
			BoosterPacks.RemoveAt(i);
			break;
		}
	}

	check(CurBoosterPack);

	PS->SetSelectPackType(CurBoosterPack);
	PS->SetPlayerState(EPlayerStateType::ITEM_SELECT);
	
	int32 CurGold = PS->GetGold();
	int32 Price = (static_cast<int32>(CurBoosterPack->GetType()) % 2) == 0 ? 6 : 4;
	
	PS->SetGold(CurGold - Price);

	VM_MainMenu->SetClearFlag(true);
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

UVM_Store* UStoreComponent::GetVMStore()
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
