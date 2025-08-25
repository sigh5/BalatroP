#include "Character/Component/StoreComponent.h"

#include "Core/MyPlayerState.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_Store.h"

#include "Item/BoosterPackData.h"

#include "PaperSprite.h"

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
	VM->OnBuyBoosterPack.AddUObject(this, &UStoreComponent::StartBoosterPackEvent);


#ifdef Store_View_TEST
	SetDownStoreItem();
	SetUpStoreItem();
#endif
}

void UStoreComponent::SetStoreView(EPlayerStateType _InType)
{
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
	auto VM_Stroe = GetVMPStore();

	JokerCards.Empty();

	int32 UpStoreItemNum = PS->GetHaveUpStoreNum();
	
	for (int i = 0; i < UpStoreItemNum; ++i)
	{
		EJokerType ItemType = SetJokerType();
		UJokerCard_Info* CurJoker = NewObject<UJokerCard_Info>();

		int32 ItemIndex = i; //static_cast<int32>(ItemType);
		FString ItemIndexStr = FString::FromInt(ItemIndex);
		FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Jokers_Sprite_%s.Jokers_Sprite_%s"), *ItemIndexStr, *ItemIndexStr);
		CurJoker->Info.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
		if (!CurJoker->Info.CardSprite.IsValid())
		{
			CurJoker->Info.CardSprite.LoadSynchronous();
		}

		CurJoker->Info.JokerType = ItemType;
		
		CurJoker->Info.Price = 3;
		JokerCards.Add(CurJoker);
	}

	VM_Stroe->SetStoreJokerData(JokerCards);
}

void UStoreComponent::SetBoucherItem()
{

}

void UStoreComponent::SetDownStoreItem()
{
	auto PS = GetPlayerState();
	auto VM_Stroe = GetVMPStore();

	int32 BoosterPackNum = PS->GetHaveBoosterPackNum();
	int32 MaxBoosterPackNum = PS->GetMaxHaveBoosterPackNum();

	if (BoosterPackNum == MaxBoosterPackNum)
	{
		BoosterPacks.Empty();
		for (int i = 0; i < BoosterPackNum; ++i)
		{
			EBoosterPackType ItemType = SetItemType();
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

EJokerType UStoreComponent::SetJokerType()
{


	return EJokerType::BASE_JOKER;
}

void UStoreComponent::StartBoosterPackEvent(UBoosterPackData* InData)
{
	auto VM_Stroe = GetVMPStore();
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

	/*
		1) 타로카드 고르는 View 키기
		2) 부스터팩 정보 가지고와서 아이템뷰에 타로 카드 넣기
	*/

	PS->SetSelectPackType(CurBoosterPack);
	PS->SetPlayerState(EPlayerStateType::ITEM_SELECT);


	VM_MainMenu->SetClearFlag(true);

	//VM_Stroe->SetBoosterPackTypes(BoosterPacks);
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
