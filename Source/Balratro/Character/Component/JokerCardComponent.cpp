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

	VM_Joker->OnSetJokerState.AddUObject(this, &UJokerCardComponent::UpdateAddJoker);
	VM_Joker->OnJokerSlotSwapData.AddUObject(this, &UJokerCardComponent::UpdateSwapJokerData);

//#ifdef JokerSlotView_VIEW_TEST
	FJokerStat Test_Data;
	EJokerType ItemType;
	int32 ItemIndex;
	FString ItemIndexStr;
	FString AssetPath;
	
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
	UpdateAddJoker(Test_Data,true);


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
	UpdateAddJoker(Test_Data, true);

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

	if (Type == EJokerType::SPADE ||
		Type == EJokerType::HEART_MUL ||
		Type == EJokerType::ACE_PLUS)
	{
		if (Type == EJokerType::SPADE)
		{
			if (CurCard->Info.SuitGrade == 1)
			{
				CurDriange += 4;

				if (IsCopy)
				{
					CurCard->PreEventJokerType.Add(EJokerType::COPY);
				}
				else
					CurCard->PreEventJokerType.Add(Type);
			}

		}
		else if (Type == EJokerType::HEART_MUL)
		{
			if (CurCard->Info.SuitGrade == 3)
			{
				if (FRandomUtils::RandomSeed.RandRange(0, 1) == 1)
				{
					CurDriange *= 1.5;

					if (IsCopy)
					{
						CurCard->PreEventJokerType.Add(EJokerType::COPY);
					}
					else
						CurCard->PreEventJokerType.Add(Type);
				}

			}
		}
		else if (Type == EJokerType::ACE_PLUS)
		{
			if (CurCard->Info.RankGrade == 1)
			{
				CurDriange += 4;
				CurChip += 30;

				if (IsCopy)
				{
					CurCard->PreEventJokerType.Add(EJokerType::COPY);
				}
				else
					CurCard->PreEventJokerType.Add(Type);
			}
		}

		JokerCard->Info.UseNum += 1;
	}
}

void UJokerCardComponent::UpdateSwapJokerData(UJokerCard_Info* Source, UJokerCard_Info* SwapDest)
{
	auto PS = GetPlayerState();

	auto CurJokerCards = PS->GetCurrentJokerCards();

	auto VM = GetVMJockerSlot();

	int32 SourceIndex = CurJokerCards.IndexOfByKey(Source);
	int32 DestIndex = CurJokerCards.IndexOfByKey(SwapDest);

	if (SourceIndex != INDEX_NONE && DestIndex != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateSwapJokerData"));
		CurJokerCards.Swap(SourceIndex, DestIndex);
	}

	PS->SetCurrentJokerCards(CurJokerCards);
	VM->SetJokerDatas(CurJokerCards);
}

void UJokerCardComponent::LastCalculatorJokerSkill(OUT int32& CurChip, OUT float& CurDriange)
{
	auto PS = GetPlayerState();
	auto CurJokerCards = PS->GetCurrentJokerCards();
	auto VM_Joker = GetVMJockerSlot();
	
	JokerGoldSum = 0;

	int32 CopyIndex = -1;
	int32 DataNum = CurJokerCards.Num();
	for (int32 i = 0; i < DataNum; ++i)
	{
		JokerGoldSum += CurJokerCards[i]->Info.Price;
		if (CurJokerCards[i]->Info.JokerType == EJokerType::COPY && i < DataNum - 1)
		{
			CopyIndex = i;
		}
	}

	if (CopyIndex == -1)
	{
		VM_Joker->SetCopyJokerSetting(EJokerType::NONE);
	}

	for (int32 i = 0; i < DataNum; ++i)
	{
		IsCopy = false;
		if (i == CopyIndex && i < DataNum - 1)
		{
			CurJokerCards[i]->Info.JokerType = CurJokerCards[i + 1]->Info.JokerType;
			IsCopy = true;
		}
		LastCalculatorJoker(CurJokerCards[i], CurChip, CurDriange);

		if (IsCopy)
		{
			CurJokerCards[i]->Info.JokerType = EJokerType::COPY;

			VM_Joker->SetCopyJokerSetting(CurJokerCards[i + 1]->Info.JokerType);
		}
	}
}

void UJokerCardComponent::CalculatorCardJokerSkill(UHandInCard_Info* CurCard, OUT int32& CurChip, OUT float& CurDriange)
{
	auto PS = GetPlayerState();
	auto CurJokerCards = PS->GetCurrentJokerCards();

	int32 CopyIndex = -1;

	UE_LOG(LogTemp, Warning, TEXT("CalculatorCardJokerSkill"));

	int32 DataNum = CurJokerCards.Num();
	for (int32 i=0; i<DataNum; ++i)
	{
		if (CurJokerCards[i]->Info.JokerType == EJokerType::COPY && i < DataNum - 1)
		{
			CopyIndex = i;
			break;
		}
	}

	for (int32 i = 0; i < DataNum; ++i)
	{
		IsCopy = false;
		if (i == CopyIndex && i < DataNum -1)
		{
			CurJokerCards[i]->Info.JokerType = CurJokerCards[i + 1]->Info.JokerType;
			
			IsCopy = true;
		}

		PlayCalculatorJoker(CurJokerCards[i], CurCard, CurChip, CurDriange);

		if (IsCopy)
		{
			CurJokerCards[i]->Info.JokerType = EJokerType::COPY;
		}
	}
}

void UJokerCardComponent::FinishRoundJokerSkill()
{
	auto PS = GetPlayerState();
	auto CurJokerCards = PS->GetCurrentJokerCards();
	auto VM = GetVMJockerSlot();

	for (auto& Joker : CurJokerCards)
	{
		if (Joker->Info.JokerType == EJokerType::EGG) 
		{
			VM->SetRoundFinishEventJoker(EJokerType::EGG);
			Joker->Info.Price += 3;
			break;
		}
	}

}

void UJokerCardComponent::UpdateAddJoker(FJokerStat& Data, bool IsAdd)
{
	auto VM_Joker = GetVMJockerSlot();
	auto PS = GetPlayerState();
	
	if (IsAdd)
	{
		PS->AddCurrentJokerCard(Data);
	}
	else
	{
		PS->DeleteCurrentJokerCard(Data);

		int32 CurPrice = Data.Price;
		int32 GetGold = PS->GetGold();
		PS->SetGold(GetGold + CurPrice);
	}


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
