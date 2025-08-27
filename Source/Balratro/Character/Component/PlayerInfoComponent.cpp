#include "Character/Component/PlayerInfoComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "Singleton/BBGameSingleton.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"

void UPlayerInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	auto& Sigleton = UBBGameSingleton::Get();
	auto PS = GetPlayerState();

	auto DataTable = Sigleton.GetDeckStatTable();
	PS->ResetDeckStatTable(DataTable);
	PS->SetCardInDeckNum(DataTable.Num());

	PS->OnSetRoundCount.AddUObject(this, &UPlayerInfoComponent::UpdateRoundCount);
	PS->OnSetEntiCount.AddUObject(this, &UPlayerInfoComponent::UpdateEntiCount);
	PS->OnSetCurrentGold.AddUObject(this, &UPlayerInfoComponent::UpdateGold);
	PS->OnPlayerUseHandPlay.AddUObject(this, &UPlayerInfoComponent::UpdateUseHandCount);
	PS->OnPlayerUseChuck.AddUObject(this, &UPlayerInfoComponent::UpdateUseChuckCount);
	PS->OnCurrentPlayerHandRanking.AddUObject(this, &UPlayerInfoComponent::UpdateHandRanking);
	PS->OnDeckCardNum.AddUObject(this, &UPlayerInfoComponent::UpdateCardInDeck);
	PS->OnSelectNextScene.AddUObject(this, &UPlayerInfoComponent::UpdateBlindInfo);
	PS->OnSetCurrentScore.AddUObject(this, &UPlayerInfoComponent::UpdateCurrentScore);

	PS->OnShowUIChip.AddUObject(this, &UPlayerInfoComponent::UpdateCalculatorChip);
	PS->OnShowUIDrainage.AddUObject(this, &UPlayerInfoComponent::UpdateCalculatorDrainage);


	Init_PlayerInfo();
}

void UPlayerInfoComponent::Init_PlayerInfo()
{
	auto PS = GetPlayerState();
	
	UpdateRoundCount();;
	UpdateEntiCount();
	UpdateGold();
	UpdateUseChuckCount(PS->GetMaxChuckCount());
	UpdateUseHandCount(PS->GetMaxHandCount());
	UpdateCardInDeck();
	UpdateBlindInfo(EPlayerStateType::NONE);
	UpdateCurrentScore();
}

UVM_PlayerInfo* UPlayerInfoComponent::GetVMPlayerInfo()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_PlayerInfo");
	Context.ContextClass = UVM_PlayerInfo::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_PlayerInfo>(Found);
}

AMyPlayerState* UPlayerInfoComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}

void UPlayerInfoComponent::UpdateCardInDeck()
{
	auto VM = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	VM->SetDeckNum(PS->GetCardInDeckNum());
}

void UPlayerInfoComponent::UpdateRoundCount()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	VM_PI->SetRoundCnt(PS->GetRoundCount());
}

void UPlayerInfoComponent::UpdateGold()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	VM_PI->SetGold(PS->GetGold());
}

void UPlayerInfoComponent::UpdateEntiCount()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	VM_PI->SetEntiCnt(PS->GetEntiCount());
}

void UPlayerInfoComponent::UpdateMaxHandCount(int16 _invalue)
{
	// 나중에 수정 필요
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	PS->SetMaxHandCount(_invalue);
}

void UPlayerInfoComponent::UpdateUseHandCount(int32 _invalue)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	int32 PlayHandCount = PS->GetMaxHandCount();
	PlayHandCount -= PS->GetUseHandCount();

	VM_PI->SetHandCount(PlayHandCount);
}

void UPlayerInfoComponent::UpdateUseChuckCount(int32 _Invalue)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	int32 ChuckCount = PS->GetMaxChuckCount();
	ChuckCount -= PS->GetUseChuckCount();

	VM_PI->SetChuckCount(ChuckCount);
}

void UPlayerInfoComponent::UpdateCurrentScore()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	VM_PI->SetScroe(PS->GetCurrentScore());
}

void UPlayerInfoComponent::UpdateMaxScore(int32 _invalue)
{
	auto VM = GetVMPlayerInfo();
	// 나중에 수정필요
}

void UPlayerInfoComponent::UpdateHandRanking()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	EPokerHand CurHandType = PS->GetCurHandCard_Type();

	FName Name;
	switch (CurHandType)
	{
	case EPokerHand::NONE:
		Name = "-";
		break;
	case EPokerHand::HIGH_CARD:
		Name = "HIGH CARD";
		break;
	case EPokerHand::ONE_PAIR:
		Name = "ONE PAIR";
		break;
	case EPokerHand::TWO_PAIR:
		Name = "TWO PAIR";
		break;
	case EPokerHand::TRIPLE:
		Name = "TRIPLE";
		break;
	case EPokerHand::STRAIGHT:
		Name = "STRAIGHT";
		break;
	case EPokerHand::FLUSH:
		Name = "FLUSH";
		break;
	case EPokerHand::FULL_HOUSE:
		Name = "FULL HOUSE";
		break;
	case EPokerHand::FOUR_CARD:
		Name = "FOUR CARD";
		break;
	case EPokerHand::STRAIGHT_FLUSH:
		Name = "STRAIGHT FLUSH";
		break;
	case EPokerHand::ROYAL_FLUSH:
		Name = "ROYAL FLUSH";
		break;
	case EPokerHand::FIVE_CARD:
		Name = "FIVE CARD";
		break;
	default:
		break;
	}

	VM_PI->SetHandName(Name);

	int32 Level = 0, BaseChip= 0 , IncreaseChip = 0, BaseDrainage = 0 , IncreaseDrainage =0;
	FString CurHandRankingName = Name.ToString().Replace(TEXT(" "), TEXT(""));
	auto CurMyHandRanking = PS->GetHandRankingInfoModify();
	for (auto CurHandRankingInfo : CurMyHandRanking)
	{
		if (CurHandRankingName == CurHandRankingInfo->_Name)
		{
			Level = CurHandRankingInfo->Info.Level;
			BaseChip = CurHandRankingInfo->Info.Chip;
			IncreaseChip = CurHandRankingInfo->Info.IncreaseChip;
			BaseDrainage = CurHandRankingInfo->Info.Drainage;
			IncreaseDrainage = CurHandRankingInfo->Info.IncreaseDrainage;

			if (PS->GetHandPlayFlag() == true)
			{
				CurHandRankingInfo->Info.UseNum++;
			}

			break;
		}
	}

	BaseChip += IncreaseChip * (Level-1);
	BaseDrainage += IncreaseDrainage * (Level - 1);

	PS->SetCurrentShowChip(BaseChip);
	PS->SetCurrentShowDrainage(BaseDrainage);
}

void UPlayerInfoComponent::UpdateBlindInfo(EPlayerStateType _InType)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();
	auto& Sigleton = UBBGameSingleton::Get();
	auto VM_MW = GetVMMainWidget();


	if (_InType == EPlayerStateType::ITEM_SELECT)
	{
		// 상점 이펙트랑 동일
		_InType = EPlayerStateType::STORE;
	}


	int32 EntiCnt = PS->GetEntiCount();
	int32 RoundCnt = PS->GetRoundCount();
	int32 BlindGrade = -1;
	int32 Reward = -1;
	int32 BlindImageIndex = -1;
	FLinearColor LinearColor(0.0f, 0.f, 0.0f, 1.0f);
	FName MainOrder = "";
	bool  BlindInfoActive = false;
	switch (_InType)
	{
	case EPlayerStateType::NONE:
		MainOrder = "Choose Your \n Next Blind";
		BlindInfoActive = false;
		break;
	case EPlayerStateType::STORE:
		MainOrder = "UPGRADE RUN!!";
		BlindInfoActive = false;
		BlindImageIndex = 2;
		break;
	case EPlayerStateType::SMALL_BLIND:
		MainOrder = "Small Blind";
		BlindInfoActive = true;
		Reward = Sigleton.GetBlindStat()[EntiCnt]->SMallReward;
		BlindGrade = Sigleton.GetBlindStat()[EntiCnt]->SMallGrade;
		++RoundCnt;
		LinearColor = FLinearColor(0.000000, 0.289068, 1.000000, 1.000000);
		BlindImageIndex = 0;
		break;
	case EPlayerStateType::SMALL_BLIND_SKIP:
		MainOrder = "Choose Your \n Next Blind";
		BlindInfoActive = false;
		break;
	case EPlayerStateType::BIG_BLIND:
		MainOrder = "Big Blind";
		BlindInfoActive = true;
		Reward = Sigleton.GetBlindStat()[EntiCnt]->BigReward;
		BlindGrade = Sigleton.GetBlindStat()[EntiCnt]->BigGrade;
		++RoundCnt;
		LinearColor = FLinearColor(1.000000, 0.928203, 0.000000, 1.000000);
		BlindImageIndex = 1;
		break;
	case EPlayerStateType::BIG_BLIND_SKIP:
		MainOrder = "Choose Your \n Next Blind";
		BlindInfoActive = false;
		break;
	case EPlayerStateType::BOSS_BLIND:
		MainOrder = "Boss Blind";
		BlindInfoActive = true;
		Reward = Sigleton.GetBlindStat()[EntiCnt]->BossReward;
		BlindGrade = Sigleton.GetBlindStat()[EntiCnt]->BossGrade;
		++RoundCnt;
		break;
	case EPlayerStateType::REWARD:
		MainOrder = "REWARD";
		BlindInfoActive = false;
		break;
	default:
		break;
	}
	
	PS->SetRoundCount(RoundCnt);
	VM_PI->SetBlindInfoActive(BlindInfoActive); // Active가 가장 먼저와야됌
	VM_PI->SetBlindGrade(BlindGrade);
	VM_PI->SetBlindReward(Reward);
	VM_PI->SetMainOrder(MainOrder);
	VM_PI->SetBlindBorderColor(LinearColor);
	VM_PI->SetBlindImageIndex(BlindImageIndex);
}

void UPlayerInfoComponent::UpdateCalculatorChip()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	int32 CurShowChip = PS->GetCurrentShowChip();

	VM_PI->SetCurChip(CurShowChip);
}

void UPlayerInfoComponent::UpdateCalculatorDrainage()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	int32 CurDrainage = PS->GetCurrentShowDrainage();

	VM_PI->SetCurDrainage(CurDrainage);
}


UVM_MainMenu* UPlayerInfoComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}
