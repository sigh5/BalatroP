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
#include "ItemSelectComponent.h"
#include "GameData/Utills.h"


void UBlindComponent::BeginPlay()
{
	Super::BeginPlay();

	auto VM_SelectBlind = GetVMBlindSelect();
	VM_SelectBlind->OnSelectBlind.AddUObject(this, &UBlindComponent::BlindSelectEvent);

	auto VM_Store = GetVMStore();
	VM_Store->OnNextButton.AddUObject(this, &UBlindComponent::StoreNextButtonClicked);

	auto PS = GetPlayerState();
	PS->OnBossSkill_PreEvent.AddUObject(this,&UBlindComponent::UseBlindBossSkill);
	PS->OnSelectNextScene.AddUObject(this, &UBlindComponent::NewtSceneEvent);
	PS->EntiBossClear.AddUObject(this, &UBlindComponent::EntiBossClear);


	ResetBlindSelectData();
}

void UBlindComponent::InitBlindSelectView()
{
	auto PS = GetPlayerState();
	auto VM = GetVMBlindSelect();
	auto BlindStatTable = UBBGameSingleton::Get().GetBlindStat();

	int32 EntiCnt = PS->GetEntiCount();
	
	auto& BossType = PS->GetCurBossType();

	if (BossType.Key != EntiCnt)
	{
		SetRandomBossType();
	}

	VM->SetBossBlindImage_AssetPath(PS->BossImagePath());
	VM->SetBossType(BossType.Value);
	int32 BossGrade = BlindStatTable[EntiCnt]->BossGrade;
	
	if (PS->GetCurBossType().Value == EBossType::WALL)
		BossGrade *= 2;
	
	VM->SetBossGrade(BossGrade);


	VM->SetSmallGrade(BlindStatTable[EntiCnt]->SMallGrade);
	VM->SetBigGrade(BlindStatTable[EntiCnt]->BigGrade);
	
	int32 SmallSkipIndex = ((EntiCnt) * 2);
	int32 BigSkipIndex = SmallSkipIndex + 1;

	VM->SetSmallBlind_SkipTag(BlindSkipTags[SmallSkipIndex]); // ���߿� �׳� 1~8���� ���� �ϱ�
	VM->SetBigBlind_SkipTag(BlindSkipTags[BigSkipIndex]);
}

void UBlindComponent::BlindSelectEvent(EPlayerStateType InValue)
{
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();
	auto VM_BlindSelect = GetVMBlindSelect();
	auto VM_CardDeck = GetVMCardDeck();

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
			VM_CardDeck->SetUseless_EmblemType(0);
		}
		else if (InValue == EPlayerStateType::BIG_BLIND)
		{
			PS->SetCurrentRoundBlindGrade(BlindStatTable[EntiCnt]->BigGrade);
			VM_CardDeck->SetUseless_EmblemType(0);
		}
		else
		{
			int32 BossGrade = BlindStatTable[EntiCnt]->BossGrade;
			
			auto CurBossType = PS->GetCurBossType().Value;
			
			if (CurBossType == EBossType::WALL)
				BossGrade *= 2;
			else if (CurBossType == EBossType::WATER)
				PS->SetUseChuckCount(PS->GetMaxChuckCount());

			PS->SetCurrentRoundBlindGrade(BossGrade);
			
			if (PS->GetCurBossType().Value == EBossType::GOAD)
			{
				VM_CardDeck->SetUseless_EmblemType(1);
			}

			bEyeSkillFirst = false;
		}

	}
	else if (InValue == EPlayerStateType::SMALL_BLIND_SKIP || InValue == EPlayerStateType::BIG_BLIND_SKIP)
	{
		VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("SelectBlindView", false));
		
		int32 CurBlindSkipTagIndex = (PS->GetEntiCount()) *2 ;

		if (InValue == EPlayerStateType::BIG_BLIND_SKIP)
			CurBlindSkipTagIndex += 1;
		
		SetBlindSkipReward(BlindSkipTags[CurBlindSkipTagIndex]);
		
		PS->SetPlayerState(EPlayerStateType::ITEM_SELECT);

		return;
	}
}

void UBlindComponent::NewtSceneEvent(EPlayerStateType InValue)
{
	if (InValue == EPlayerStateType::BLINDSELECT)
	{
		BlindViewActive();
	}
	else if (InValue == EPlayerStateType::RESET_GAME)
	{
		ResetBlindSelectData();
	}

}

void UBlindComponent::StoreNextButtonClicked()
{
	auto PS = GetPlayerState();
	PS->SetPlayerState(EPlayerStateType::BLINDSELECT);
}

void UBlindComponent::BlindViewActive()
{
	auto VM_MainWidget = GetVMMainWidget();
	auto PS = GetPlayerState();

	VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("StoreView", false));
	VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("SelectBlindView", true));

	InitBlindSelectView();
}

void UBlindComponent::ResetBlindSelectData()
{
	PreBossSkill.Empty();
	RandomArray.Empty();
	BlindSkipTags.Empty();

	auto PS = GetPlayerState();
	PreBossSkill.Add(EBossType::HOOK, [this]() { HOOK_Skill(); });
	PreBossSkill.Add(EBossType::OX, [this]() { OX_Skill(); });
	PreBossSkill.Add(EBossType::WALL, [this]() { WALL_SKill(); });
	PreBossSkill.Add(EBossType::ARM, [this]() { ARM_Skill(); });
	PreBossSkill.Add(EBossType::PSYCHIC, [this]() { PSYCHIC_Skill(); });
	PreBossSkill.Add(EBossType::GOAD, [this]() { GOAD_Skill(); });
	PreBossSkill.Add(EBossType::WATER, [this]() { WATER_Skill(); });
	PreBossSkill.Add(EBossType::EYE, [this]() { EYE_Skill(); });

	for (int32 i = 1; i <= 8; i++)
	{
		RandomArray.Add(i);
	}

	int32 NumElements = RandomArray.Num();
	for (int32 i = NumElements - 1; i > 0; --i)
	{
		int32 RandomIndex = FRandomUtils::RandomSeed.RandRange(0, i);

		if (i != RandomIndex)
		{
			RandomArray.Swap(i, RandomIndex);
		}
	}
	
	PS->SetCurBossType({ -1,EBossType::NONE });
	BlindSkipTags.Add(EBlindSkip_Tag::ARCANA_PACK);  // �ϴ� �� �׽�Ʈ��
	BlindSkipTags.Add(EBlindSkip_Tag::ARCANA_PACK);
	BlindSkipTags.Add(EBlindSkip_Tag::ARCANA_PACK); 
	BlindSkipTags.Add(EBlindSkip_Tag::ARCANA_PACK);
	// �ϴ� �� �׽�Ʈ��
	InitBlindSelectView();
}

void UBlindComponent::SetRandomBossType()
{
	auto PS = GetPlayerState();
	auto VM = GetVMBlindSelect();
	TPair<int32, EBossType> MyBossType;
	
	int32 EntiCount = PS->GetEntiCount();
	MyBossType = { EntiCount ,static_cast<EBossType>(RandomArray[EntiCount])};
	
	RandomArray.RemoveAt(EntiCount);

	// TEST
	MyBossType = { EntiCount , EBossType::GOAD };

	PS->SetCurBossType(MyBossType);
}

void UBlindComponent::UseBlindBossSkill()
{
	auto PS = GetPlayerState();

	if (EPlayerStateType::BOSS_BLIND != PS->GetPlayerState())
	{
		return;
	}

	EBossType CurBossType = PS->GetCurBossType().Value;
	PreBossSkill[CurBossType]();
}

void UBlindComponent::SetBlindSkipReward(EBlindSkip_Tag CurTagType)
{
	auto PS = GetPlayerState();
	PS->SetCurBlindSkipReward(CurTagType);
}

void UBlindComponent::EntiBossClear()
{
	auto VM = GetVMBlindSelect();
	VM->GetEntiBossClearFlag();

	InitBlindSelectView();
}

void UBlindComponent::HOOK_Skill()
{
	auto PS = GetPlayerState();
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
	
	PS->SetRestCardInHands(RestHands); // �����ִ� 
}

void UBlindComponent::OX_Skill()
{
	auto PS = GetPlayerState();
	
	auto CurHandType = PS->GetCurHandCard_Type();
	
	if (PS->MostUseHandRankingName()->_Type == CurHandType)
	{
		PS->SetBossSkill_GoldZero(0);
	}
}

void UBlindComponent::ARM_Skill()
{
	auto PS = GetPlayerState();

	auto CurHandType = PS->GetCurHandCard_Type();

	auto& MyHandRankingInfos = PS->GetHandRankingInfoModify();

	for (auto Info : MyHandRankingInfos)
	{
		if (Info->_Type == CurHandType)
		{
			if(Info->Info.Level > 1)
				Info->Info.Level -= 1;

			break;
		}
	}
}

void UBlindComponent::PSYCHIC_Skill()
{
	auto PS = GetPlayerState();

	auto VM_CardDeck = GetVMCardDeck();

	bool CurPlayCardMax = VM_CardDeck->GetIsSelectedMax();

	if (CurPlayCardMax ==false)
	{
		PS->SetHandPlayFlag(false);
	}

}

void UBlindComponent::EYE_Skill()
{
	auto PS = GetPlayerState();
	auto CurHandType = PS->GetCurHandCard_Type();

	if (EyeSkillArr.Num() == 0)
	{
		EyeSkillArr.Add(CurHandType);
		return;
	}
	else
	{
		if (EyeSkillArr.Contains(CurHandType) == true)
		{
			PS->SetHandPlayFlag(false);
		}
		else
		{
			EyeSkillArr.Add(CurHandType);
		}
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