#include "Character/Component/PlayerInfoComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>


#include "Singleton/BBGameSingleton.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"


void UPlayerInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	auto& Sigleton = UBBGameSingleton::Get();
	auto PS = GetPlayerState();

	auto DataTable = Sigleton.GetDeckStatTable();
	PS->ResetDeckStatTable(DataTable);
	PS->SetCardInDeckNum(DataTable.Num());

	PS->OnPlayerUseChuck.AddUObject(this, &UPlayerInfoComponent::UpdateUseChuckCount);
	PS->OnPlayerUseHandPlay.AddUObject(this, &UPlayerInfoComponent::UpdateUseHandCount);
	PS->OnCurrentPlayerHandRanking.AddUObject(this, &UPlayerInfoComponent::UpdateHandRanking);
	PS->OnDeckCardNum.AddUObject(this, &UPlayerInfoComponent::UpdateCardInDeck);

	UpdateUseHandCount(PS->GetMaxHandCount());
	UpdateUseChuckCount(PS->GetMaxChuckCount());
	UpdateGold(PS->GetGold());
	UpdateEntiCount(PS->GetEntiCount());
	UpdateRoundCount(PS->GetRoundCount());;
	UpdateCardInDeck();
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

void UPlayerInfoComponent::UpdateRoundCount(int16 _invalue)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	PS->SetRoundCount(_invalue);
	VM_PI->SetRoundCnt(PS->GetRoundCount());
}

void UPlayerInfoComponent::UpdateGold(int16 _invalue)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	PS->SetGold(_invalue);
	VM_PI->SetGold(PS->GetGold());
}

void UPlayerInfoComponent::UpdateEntiCount(int16 _invalue)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	PS->SetEntiCount(_invalue);
	VM_PI->SetEntiCnt(PS->GetEntiCount());
}

void UPlayerInfoComponent::UpdateMaxHandCount(int16 _invalue)
{
	/*auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	PS->SetMaxHandCount(_invalue);
	VM_PI->SetHand(PS->GetMaxHandCount());*/
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

void UPlayerInfoComponent::UpdateCurrentScore(int32 _invalue)
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	//PS->SetCurrentScore(_invalue);
	//VM_PI->SetRoundCnt(PS->GetCurrentScore());
}

void UPlayerInfoComponent::UpdateMaxScore(int32 _invalue)
{
	auto VM = GetVMPlayerInfo();
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

	int32 Level = 0, BaseChip= 0 , IncreaseChip = 0, Drainage = 0;
	FString StrA = Name.ToString().Replace(TEXT(" "), TEXT(""));
	auto Temp = PS->GetHandRankingInfo();
	for (auto CurInfo : Temp)
	{
		if (StrA == CurInfo->_Name)
		{
			Level = CurInfo->Info.Level;
			BaseChip = CurInfo->Info.Chip;
			IncreaseChip = CurInfo->Info.IncreaseChip;

			break;
		}
	}
	
	BaseChip += IncreaseChip * (Level-1);

	VM_PI->SetCurChip(BaseChip);
	

}
