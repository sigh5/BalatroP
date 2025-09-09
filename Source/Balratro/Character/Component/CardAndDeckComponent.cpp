#include "Character/Component/CardAndDeckComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"


#include "Singleton/BBGameSingleton.h"
#include "Core/MyPlayerState.h"

#include "UI/MVVM/ViewModel/VM_CardDeck.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_ItemSelect.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

#include "GameData/HandRankingStat.h"
#include "Interface/CalculatorScoreInterface.h"
#include "GameData/Utills.h"

void UCardAndDeckComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto VM = GetVMCardDeck();
	const auto VM_ItemSelcet = GetVMItemSelect();
	const auto VM_Joker = GetVMJoker();

	auto	PS = GetPlayerState();
	PS->OnSelectNextScene.AddUObject(this, &UCardAndDeckComponent::SetVisibleCardDeckView);

	VM->OnSortTypeChange.AddUObject(this, &UCardAndDeckComponent::SortHandInCard);
	VM->OnUseChuck.AddUObject(this, &UCardAndDeckComponent::UpdateChuck);
	VM->OnUseHandPlay.AddUObject(this, &UCardAndDeckComponent::UpdateHandPlay);
	VM->OnSwapCards.AddUObject(this, &UCardAndDeckComponent::SwapCardOrder);
	VM_Joker->OnEffectUIViewFinish.AddUObject(this, &UCardAndDeckComponent::AllEffectViewFinish);
	VM_ItemSelcet->OnUseTaroCard.AddUObject(this, &UCardAndDeckComponent::UseTaroItem);
}

void UCardAndDeckComponent::UpdateCardInHand(TArray<UHandInCard_Info*>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	auto CurHandInCard = PS->GetCurrentAllHands();
	CurHandInCard.RemoveAll([&](UHandInCard_Info* HandCard)
		{
			if (HandCard == nullptr)
				return false;

			return _DeckCardStat.ContainsByPredicate([&](const UHandInCard_Info* Stat)
				{
					return Stat->Info == HandCard->Info;
				});
		});
	// 나중에 DeckNumIndex ==> 카드가 덱에 추가된 순서를 csv에 넣어줘야함

	PS->SetCurrentAllHands(CurHandInCard);

	TArray<UHandInCard_Info*> Empty;
	PS->SetCurCalculatorCardInHands(Empty);

}

void UCardAndDeckComponent::SetVisibleCardDeckView(EPlayerStateType InValue)
{
	auto VM_MainMenu = GetVMMainWidget();
	auto VM_CardDeck = GetVMCardDeck();

	if (InValue == EPlayerStateType::SMALL_BLIND || InValue == EPlayerStateType::BIG_BLIND || InValue == EPlayerStateType::BOSS_BLIND)
	{
		FASTLOGTEXT;
		VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", true));
		VM_CardDeck->SetItemSelectFlag(false);
		InitDeck();
	}
	else if (InValue == EPlayerStateType::ITEM_SELECT)
	{
		VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", true));
		VM_CardDeck->SetItemSelectFlag(true);
		InitDeck();
	}
	else if (InValue == EPlayerStateType::RESET_GAME)
	{
		DeckCardStat.Empty();
		CurDrawIndex = 0;
		_DelayTime = 0.f;
		ResultScore = 0.f;
		nCardNum = 0;
	}
	else
		return;
}

void UCardAndDeckComponent::FinishHandPlay()
{
	auto PS = GetPlayerState();
	
	int32 SumScore = PS->GetCurrentScore();
	int32 CurrentSum = ResultScore + SumScore;
	
	PS->SetMaxScore(ResultScore);

	if (CurrentSum >= PS->GetCurrentRoundBlindGrade())
	{
		ResultScore = 0;	
		PS->SetCurrentScore(CurrentSum);

		GetWorld()->GetTimerManager().ClearTimer(TotalScoreHandle);

		auto VM_MainWidget = GetVMMainWidget();
		VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", false));

		PS->SetPlayerState(EPlayerStateType::REWARD);
		GetWorld()->GetTimerManager().ClearTimer(ItemSkipTimerHandle);	// 게임 끝나면 초기화 하는게 맞는듯? (마땅한 자리가 없음)
	}
	else
	{
		if (PS->GetUseHandCount() == PS->GetMaxHandCount())
		{
			// 게임 종료 뷰 띄우기
			UE_LOG(LogTemp, Warning, TEXT("FinishHandPlay_HandPlayNumNone0"));
			PS->SetPlayerState(EPlayerStateType::GAME_OVER);

		}
		else
		{
			PS->SetCurrentScore(CurrentSum);
			UpdateCardInHand(DeckCardStat);
			DrawCard(nCardNum);
		}
	}
}

void UCardAndDeckComponent::SwapCardOrder(UHandInCard_Info* SwapDest, UHandInCard_Info* Source)
{
	auto PS = GetPlayerState();	
	auto CurAllHands = PS->GetCurrentAllHands();

	int32 SourceIndex = CurAllHands.IndexOfByKey(Source);
	int32 DestIndex = CurAllHands.IndexOfByKey(SwapDest);

	if (SourceIndex != INDEX_NONE && DestIndex != INDEX_NONE)
	{
		CurAllHands.Swap(SourceIndex, DestIndex);
	}

	PS->SetCurrentAllHands(CurAllHands);
}

void UCardAndDeckComponent::UseTaroItem(FTaroStat& TaroStat)
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();

	auto TaroTable = PS->GetTaroStatTableModify();

	int32 TaroSelectCount = PS->GetCurSelectTaroNum();

	for (auto& Info : TaroTable)
	{
		if (Info->Info.index == TaroStat.index)
		{
			Info->Info.useNum++;
			break;
		}
	}
	
	if (TaroStat.EnforceType != 0)
	{
		UseEnhanceTaro(TaroStat.EnforceType);
	}
	else if (TaroStat.SealType != 0)
	{
		UseSealTaro(TaroStat.SealType);
	}
	else if (TaroStat.GhostCardType != 0)
	{
		UseGhostTaro(TaroStat.GhostCardType);
	}

	VM->SetCurrentAllHands(PS->GetCurrentAllHands());

	// 현재 부스터팩에서 사용할 타로 횟수가 끝나면 자동으로 상점으로 변환
	if (TaroSelectCount - 1 == 0) 
	{
		GetWorld()->GetTimerManager().SetTimer(
			ItemSkipTimerHandle,
			[VM]()
			{
				if (VM)
				{
					VM->SkipButtonClicked();
				}
			},
			2.5f,
			false
		);
	}
	else
	{
		PS->SetCurSelectTaroNum(TaroSelectCount - 1);
		GetWorld()->GetTimerManager().ClearTimer(TotalScoreHandle);
	}

}

void UCardAndDeckComponent::UseEnhanceTaro(int32 EnhanceType)
{
	auto PS = GetPlayerState();
	auto& Cards = PS->GetCurCalculatorCardInHands();

	for (auto& Card : Cards)
	{
		if (EnhanceType >= 9)
		{
			FName CurName = Card->Info.Name;
			FString NameStr = CurName.ToString();

			if (static_cast<ETaroSkillType>(EnhanceType) == ETaroSkillType::CHANGE_HEART)
			{
				NameStr[0] = 'H';
				Card->Info.SuitGrade = 3;
			}
			else if (static_cast<ETaroSkillType>(EnhanceType) == ETaroSkillType::CHANGE_SPADE)
			{
				NameStr[0] = 'S';
				Card->Info.SuitGrade = 1;
			}
			else if (static_cast<ETaroSkillType>(EnhanceType) == ETaroSkillType::CHANGE_CLOVER)
			{
				NameStr[0] = 'C';
				Card->Info.SuitGrade = 4;
			}
			else if (static_cast<ETaroSkillType>(EnhanceType) == ETaroSkillType::CHANGE_DIAMOND)
			{
				NameStr[0] = 'D';
				Card->Info.SuitGrade = 2;
			}

			Card->Info.Name = FName(*NameStr);
			
			FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Card/%s.%s"), *Card->Info.Name.ToString(), *Card->Info.Name.ToString());
			Card->Info.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
			
			if (!Card->Info.CardSprite.IsValid())
				Card->Info.CardSprite.LoadSynchronous();

		}
		else
		{
			Card->Info.EnforceType = static_cast<ETaroSkillType>(EnhanceType);
		}
	}
}

void UCardAndDeckComponent::UseSealTaro(int32 SealType)
{
}

void UCardAndDeckComponent::UseGhostTaro(int32 GhostType)
{
}

void UCardAndDeckComponent::AllEffectViewFinish()
{
	FTimerDelegate Delegate;
	Delegate.BindLambda([&]()
		{
			FinishHandPlay();
		});

	GetWorld()->GetTimerManager().SetTimer(TotalScoreHandle, Delegate, 1.5f, false);
}

void UCardAndDeckComponent::SetPlayCardEffect()
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();
	
	auto CurPlayCards = PS->GetCurCalculatorCardInHands();
	auto CurRestCards = PS->GetRestCardInHands();
	
	VM->SetIsHandPlayFlag(PS->GetHandPlayFlag());
	VM->SetRestCardDatas(CurRestCards);

	if (PS->GetPlayerState() == EPlayerStateType::BOSS_BLIND)
	{
		auto CurBossType = PS->GetCurBossType();
		VM->SetBossSkillUse(true); 
	}
	else
	{
		VM->SetBossSkillUse(false);
	}

	VM->SetCurCardsData(CurPlayCards); 
	VM->SetRestCardEffectFlag(true);

	PS->SetHandPlayFlag(false);
}

void UCardAndDeckComponent::ShuffleDeck()
{
	auto PS = GetPlayerState();
	auto& MyDeckStatTable = PS->GetDeckStatTableModify();

	int32 NumElements = MyDeckStatTable.Num();
	for (int32 i = NumElements - 1; i > 0; --i)
	{
		int32 RandomIndex = FRandomUtils::RandomSeed.RandRange(0, i);
		if (i != RandomIndex)
		{
			MyDeckStatTable.Swap(i, RandomIndex);
		}
	}
}

void UCardAndDeckComponent::DrawCard(int32 DrawCardNum)
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();
	auto& MyDeckStatTable = PS->GetDeckStatTableModify();

	for (int i = CurDrawIndex; i < CurDrawIndex + DrawCardNum; ++i)
	{
		if (!MyDeckStatTable[i]->Info.CardSprite.IsValid())
		{
			MyDeckStatTable[i]->Info.CardSprite.LoadSynchronous();
		}

		PS->AddHandInCard(MyDeckStatTable[i]);
	}
	SortHandInCard(PS->GetCurSortType());

	VM->SetIsSelectedMax(false);

	PS->SetCardInDeckNum(PS->GetCardInDeckNum() - DrawCardNum);
	CurDrawIndex += DrawCardNum;
}

void UCardAndDeckComponent::UpdateChuck(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	int32 CurChuckCnt = PS->GetUseChuckCount();
	if (PS->GetMaxChuckCount() < CurChuckCnt + 1)
	{
		return;
	}

	PS->SetAllChuckCount();
	PS->SetUseChuckCount(++CurChuckCnt);

	UpdateCardInHand(_DeckCardStat);
	DrawCard(CardNum);
}

void UCardAndDeckComponent::UpdateHandPlay(int32 CardNum, TArray<UHandInCard_Info*>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	int32 MaxCount = PS->GetMaxHandCount();
	int32 UseCount = PS->GetUseHandCount();

	if (MaxCount < UseCount + 1)
		return;

	for (UActorComponent* Comp : GetOwner()->GetComponents())
	{
		if (Comp->GetClass()->ImplementsInterface(UCalculatorScoreInterface::StaticClass()))
		{
			ICalculatorScoreInterface* InterfacePtr = Cast<ICalculatorScoreInterface>(Comp);
			if (InterfacePtr)
			{
				ResultScore = InterfacePtr->CalCulatorHandRanking(CardNum, _DeckCardStat);
				SetPlayCardEffect();

				DeckCardStat = _DeckCardStat;
				nCardNum = CardNum;

				break;
			}
		}
	}

	PS->SetUseHandCount(UseCount + 1);
}

void UCardAndDeckComponent::InitDeck()
{
	CurDrawIndex = 0;
	_DelayTime = 0.f;
	ResultScore = 0;

	ShuffleDeck();
	DrawCard(8);

	auto VM = GetVMCardDeck();
	auto PS = GetPlayerState();

	if (PS->GetPlayerState() == EPlayerStateType::BOSS_BLIND)
	{
		VM->SetCurrentBossType(PS->GetCurBossType().Value);
	}
	else
	{
		VM->SetCurrentBossType(EBossType::NONE);
	}

}

void UCardAndDeckComponent::SortHandInCard(const EHandInCardSortType& InType)
{
	auto PS = GetPlayerState();
	auto CurHandInCard = PS->GetCurrentAllHands();
	const auto VM = GetVMCardDeck();

	if (InType == EHandInCardSortType::SORT_RANK)
	{
		CurHandInCard.Sort([&](const auto& A, const auto& B)
			{
				if (A.Info.RankGrade == B.Info.RankGrade)
				{
					return A.Info.SuitGrade < B.Info.SuitGrade;
				}

				return A.Info.RankGrade < B.Info.RankGrade;
			});

	}
	else
	{
		CurHandInCard.Sort([&](const auto& A, const auto& B)
			{
				if (A.Info.SuitGrade == B.Info.SuitGrade)
				{
					return A.Info.RankGrade < B.Info.RankGrade;
				}

				return A.Info.SuitGrade < B.Info.SuitGrade;
			});
	}

	PS->SetCurrentAllHands(CurHandInCard);

	VM->SetCurrentAllHands(PS->GetCurrentAllHands());
}

UVM_CardDeck* UCardAndDeckComponent::GetVMCardDeck()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_CardDeck");
	Context.ContextClass = UVM_CardDeck::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_CardDeck>(Found);
}

UVM_MainMenu* UCardAndDeckComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}


AMyPlayerState* UCardAndDeckComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}


UVM_ItemSelect* UCardAndDeckComponent::GetVMItemSelect()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_ItemSelect");
	Context.ContextClass = UVM_ItemSelect::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_ItemSelect>(Found);
}

UVM_JockerSlot* UCardAndDeckComponent::GetVMJoker()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_JockerSlot");
	Context.ContextClass = UVM_JockerSlot::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_JockerSlot>(Found);
}
