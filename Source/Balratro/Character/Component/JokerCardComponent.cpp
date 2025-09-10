// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/JokerCardComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "GameData/Utills.h"


// 디버그용
#include "PaperSprite.h"

void UJokerCardComponent::BeginPlay()
{
	Super::BeginPlay();

	auto VM_Joker = GetVMJockerSlot();
	auto PS = GetPlayerState();

	VM_Joker->OnAddJoker.AddUObject(this, &UJokerCardComponent::UpdateAddJoker);

	// BASE_JOKER, LAST_HAND_MUL3, PAIR_MUL_DRANGE, JOKER_GOLD_SUM, PAIR_CHIP,POP_CONE ,PAIR_DRAINAGE
//#ifdef JokerSlotView_VIEW_TEST
	FJokerStat Test_Data;
	EJokerType ItemType = EJokerType::BASE_JOKER;
	
	int32 ItemIndex = static_cast<int32>(EJokerType::BASE_JOKER);
	FString ItemIndexStr = FString::FromInt(ItemIndex);
	FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Joker%s.Joker%s"), *ItemIndexStr, *ItemIndexStr);
	Test_Data.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Test_Data.CardSprite.IsValid())
	{
		Test_Data.CardSprite.LoadSynchronous();
	}

	Test_Data.JokerType = ItemType;
	Test_Data.Price = 3;
	UpdateAddJoker(Test_Data);

	ItemType = EJokerType::SPADE;
	ItemIndex = static_cast<int32>(EJokerType::SPADE);
	ItemIndexStr = FString::FromInt(ItemIndex);
	AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Joker%s.Joker%s"), *ItemIndexStr, *ItemIndexStr);
	Test_Data.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Test_Data.CardSprite.IsValid())
	{
		Test_Data.CardSprite.LoadSynchronous();
	}

	Test_Data.JokerType = ItemType;
	Test_Data.Price = 5;
	UpdateAddJoker(Test_Data);


	ItemType = EJokerType::GOLD;
	ItemIndex = static_cast<int32>(EJokerType::GOLD);
	ItemIndexStr = FString::FromInt(ItemIndex);
	AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Joker%s.Joker%s"), *ItemIndexStr, *ItemIndexStr);
	Test_Data.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Test_Data.CardSprite.IsValid())
	{
		Test_Data.CardSprite.LoadSynchronous();
	}

	Test_Data.JokerType = ItemType;
	Test_Data.Price = 5;
	UpdateAddJoker(Test_Data);


	ItemType = EJokerType::COPY;
	ItemIndex = static_cast<int32>(EJokerType::COPY);
	ItemIndexStr = FString::FromInt(ItemIndex);
	AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Joker%s.Joker%s"), *ItemIndexStr, *ItemIndexStr);
	Test_Data.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Test_Data.CardSprite.IsValid())
	{
		Test_Data.CardSprite.LoadSynchronous();
	}

	Test_Data.JokerType = ItemType;
	Test_Data.Price = 5;
	UpdateAddJoker(Test_Data);


	ItemType = EJokerType::ACE_PLUS;
	ItemIndex = static_cast<int32>(EJokerType::ACE_PLUS);
	ItemIndexStr = FString::FromInt(ItemIndex);
	AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Joker%s.Joker%s"), *ItemIndexStr, *ItemIndexStr);
	Test_Data.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (!Test_Data.CardSprite.IsValid())
	{
		Test_Data.CardSprite.LoadSynchronous();
	}

	Test_Data.JokerType = ItemType;
	Test_Data.Price = 5;
	UpdateAddJoker(Test_Data);

//#endif
}


void UJokerCardComponent::LastCalculatorJoker(UJokerCard_Info* JokerCard, OUT int32& CurChip, OUT float& CurDriange)
{
	auto PS = GetPlayerState();
	auto VM = GetVMJockerSlot();
	EJokerType Type = JokerCard->Info.JokerType;

	if (false ==(Type == EJokerType::BASE_JOKER || Type == EJokerType::LAST_HAND_MUL3 || Type == EJokerType::PAIR_MUL_DRANGE
		|| Type == EJokerType::PAIR_DRAINAGE || Type == EJokerType::JOKER_GOLD_SUM || Type == EJokerType::PAIR_CHIP
		|| Type == EJokerType::POP_CONE ))
	{
		return;
	}

	bool IsLastHand = ( (PS->GetUseHandCount() + 1) == PS->GetMaxHandCount()) ? true : false;
	VM->SetIsLastHandPlay(IsLastHand);
	VM->SetCurrentPokerHand(PS->GetCurHandCard_Type());

	
	if (Type == EJokerType::BASE_JOKER)
	{
		CurDriange += 4;
	}
	else if (Type == EJokerType::LAST_HAND_MUL3)
	{
		if (IsLastHand)
		{
			CurDriange *= 3;
		}
	}
	else if (Type == EJokerType::PAIR_MUL_DRANGE)
	{
		if (PS->GetCurHandCard_Type() == EPokerHand::ONE_PAIR)
		{
			CurDriange *= 2;
		}
	}
	else if (Type == EJokerType::PAIR_DRAINAGE)
	{
		if (PS->GetCurHandCard_Type() == EPokerHand::ONE_PAIR)
		{
			CurDriange += 4;
		}
	}
	else if (Type == EJokerType::JOKER_GOLD_SUM)
	{
		JokerGoldSum -= JokerCard->Info.Price;
		CurDriange += JokerGoldSum;

		VM->SetAddtionalValue(JokerGoldSum);
	}
	else if (Type == EJokerType::PAIR_CHIP)
	{
		if (PS->GetCurHandCard_Type() == EPokerHand::ONE_PAIR)
			CurChip += 30;
	}
	else if (Type == EJokerType::POP_CONE)
	{
		float Base = 200;
		Base *= 0.01;
		CurDriange *= Base;
		VM->SetAddtionalValue(Base);
		Base -= (JokerCard->Info.UseNum + 1);
	}

	JokerCard->Info.UseNum += 1;
}

void UJokerCardComponent::PlayCalculatorJoker(UJokerCard_Info* JokerCard, UHandInCard_Info* CurCard, OUT int32& CurChip, OUT float& CurDriange)
{
	auto PS = GetPlayerState();
	auto VM = GetVMJockerSlot();
	EJokerType Type = JokerCard->Info.JokerType;

	if (false == (Type == EJokerType::SPADE || Type == EJokerType::HEART_MUL))
	{
		return;
	}

	if (Type == EJokerType::SPADE)
	{
		if (CurCard->Info.SuitGrade == 1)
		{
			CurDriange += 4;
			CurCard->PreEventJokerType = Type;
		}

	}
	else if (Type == EJokerType::HEART_MUL)
	{
		if (CurCard->Info.SuitGrade == 3)
		{
			if (FRandomUtils::RandomSeed.RandRange(0, 1) == 1)
			{
				CurDriange *= 1.5;
				CurCard->PreEventJokerType = Type;
			}
			
		}
	}

}

void UJokerCardComponent::LastCalculatorJokerSkill(OUT int32& CurChip, OUT float& CurDriange)
{
	auto PS = GetPlayerState();
	auto CurJokerCards = PS->GetCurrentJokerCards();

	JokerGoldSum = 0;
	for (auto Joker : CurJokerCards)
	{
		JokerGoldSum += Joker->Info.Price;
	}

	for (auto Joker : CurJokerCards)
	{
		LastCalculatorJoker(Joker, CurChip, CurDriange);
	}
}

void UJokerCardComponent::CalculatorCardJokerSkill(UHandInCard_Info* CurCard, OUT int32& CurChip, OUT float& CurDriange)
{
	auto PS = GetPlayerState();
	auto CurJokerCards = PS->GetCurrentJokerCards();


	for (auto Joker : CurJokerCards)
	{
		PlayCalculatorJoker(Joker, CurCard,CurChip, CurDriange);
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
