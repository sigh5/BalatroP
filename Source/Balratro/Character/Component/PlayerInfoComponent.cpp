#include "Character/Component/PlayerInfoComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Core/MyPlayerState.h"

#include "Singleton/BBGameSingleton.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"

void UPlayerInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PS = GetPlayerState();

	PS->OnSetRoundCount.AddUObject(this, &UPlayerInfoComponent::UpdateRoundCount);
	PS->OnSetEntiCount.AddUObject(this, &UPlayerInfoComponent::UpdateEntiCount);
	PS->OnSetCurrentGold.AddUObject(this, &UPlayerInfoComponent::UpdateGold);
	PS->OnPlayerUseHandPlay.AddUObject(this, &UPlayerInfoComponent::UpdateUseHandCount);
	PS->OnPlayerUseChuck.AddUObject(this, &UPlayerInfoComponent::UpdateUseChuckCount);
	PS->OnCurrentPlayerHandRanking.AddUObject(this, &UPlayerInfoComponent::UpdateHandRanking);
	PS->OnDeckCardNum.AddUObject(this, &UPlayerInfoComponent::UpdateCardInDeck);
	PS->OnSelectNextScene.AddUObject(this, &UPlayerInfoComponent::UpdateScene_PlayerInfo);
	PS->OnSetCurrentScore.AddUObject(this, &UPlayerInfoComponent::UpdateCurrentScore);
	PS->OnShowUIChip.AddUObject(this, &UPlayerInfoComponent::UpdateCalculatorChip);
	PS->OnShowUIDrainage.AddUObject(this, &UPlayerInfoComponent::UpdateCalculatorDrainage);

	Init_PlayerInfo();
}

void UPlayerInfoComponent::Init_PlayerInfo()
{
	auto PS = GetPlayerState();
	auto& Sigleton = UBBGameSingleton::Get();
	auto DataTable = Sigleton.GetDeckStatTable();
	
	PS->ResetDeckStatTable(DataTable);
	PS->SetCardInDeckNum(DataTable.Num());

	UpdateRoundCount();;
	UpdateEntiCount();
	UpdateGold();
	UpdateUseChuckCount(PS->GetMaxChuckCount());
	UpdateUseHandCount(PS->GetMaxHandCount());
	UpdateCardInDeck();
	UpdateScene_PlayerInfo(EPlayerStateType::NONE);
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
	// ���߿� ���� �ʿ�
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

void UPlayerInfoComponent::UpdateHandRanking()
{
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();

	EPokerHand CurHandType = PS->GetCurHandCard_Type();

	int32 Level = 0, BaseChip = 0, IncreaseChip = 0, BaseDrainage = 0, IncreaseDrainage = 0;
	FName Name ="-";

	if (CurHandType != EPokerHand::NONE)
	{
		auto& CurMyHandRanking = PS->GetHandRankingInfoModify();
		for (auto& CurHandRankingInfo : CurMyHandRanking)
		{
			if (CurHandRankingInfo->Find(CurHandType))
			{
				Name = CurHandRankingInfo->_Name;
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
		BaseChip += IncreaseChip * (Level - 1);
		BaseDrainage += IncreaseDrainage * (Level - 1);
	}
	
	VM_PI->SetHandName(Name);
	
	PS->SetCurrentShowChip(BaseChip);
	PS->SetCurrentShowDrainage(BaseDrainage);
}

void UPlayerInfoComponent::UpdateScene_PlayerInfo(EPlayerStateType _InType)
{
	if (_InType == EPlayerStateType::RESET_GAME)
	{
		Init_PlayerInfo();
		return;
	}
	
	auto VM_PI = GetVMPlayerInfo();
	auto PS = GetPlayerState();
	auto& Sigleton = UBBGameSingleton::Get();
	auto VM_MW = GetVMMainWidget();

	if (_InType == EPlayerStateType::ITEM_SELECT)
	{
		_InType = EPlayerStateType::STORE;  	// ���� ����Ʈ�� ����
	}

	int32 EntiCnt = PS->GetEntiCount();
	int32 RoundCnt = PS->GetRoundCount();
	int32 BlindGrade = -1;
	int32 Reward = -1;
	FString BlindImageMatPath = TEXT("");
	FLinearColor LinearColor(0.0f, 0.f, 0.0f, 1.0f);
	FName MainOrder = "";
	bool  BlindInfoActive = false;
	switch (_InType)
	{
	case EPlayerStateType::NONE:
		MainOrder = "Choose Your \n Next Blind";
		BlindInfoActive = false;
		break;
	case EPlayerStateType::BLINDSELECT:
		MainOrder = "Choose Your \n Next Blind";
		BlindInfoActive = false;
		break;
	case EPlayerStateType::STORE:
		MainOrder = "UPGRADE RUN!!";
		BlindInfoActive = false;
		BlindImageMatPath = TEXT("Store");
		break;
	case EPlayerStateType::SMALL_BLIND:
		MainOrder = "Small Blind";
		BlindInfoActive = true;
		Reward = Sigleton.GetBlindStat()[EntiCnt]->SMallReward;
		BlindGrade = Sigleton.GetBlindStat()[EntiCnt]->SMallGrade;
		++RoundCnt;
		LinearColor = FLinearColor(0.000000, 0.289068, 1.000000, 1.000000);
		BlindImageMatPath = TEXT("/Game/UI/View/PlayerInfo/M_SmallBlind.M_SmallBlind");
		UE_LOG(LogTemp, Warning, TEXT("SMALL_BLIND : %d"), Reward);
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
		BlindImageMatPath = TEXT("/Game/UI/View/PlayerInfo/M_BigBlind.M_BigBlind");
		break;
	case EPlayerStateType::BIG_BLIND_SKIP:
		MainOrder = "Choose Your \n Next Blind";
		BlindInfoActive = false;
		break;
	case EPlayerStateType::BOSS_BLIND:
		MainOrder = FName(*(PS->BossTypeToString()));
		BlindInfoActive = true;
		Reward = Sigleton.GetBlindStat()[EntiCnt]->BossReward;
		BlindGrade = Sigleton.GetBlindStat()[EntiCnt]->BossGrade;
		BlindImageMatPath = PS->BossImagePath();
		LinearColor = FLinearColor(1.000000, 1.0, 1.0, 1.000000);
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
	VM_PI->SetBlindInfoActive(BlindInfoActive); // Active�� ���� �����;߉�
	VM_PI->SetBlindGrade(BlindGrade);
	VM_PI->SetBlindReward(Reward);
	VM_PI->SetMainOrder(MainOrder);
	VM_PI->SetBlindBorderColor(LinearColor);
	
	if (BlindImageMatPath != "")
	{
		VM_PI->SetBlindMaterialPath(BlindImageMatPath);
	}
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

