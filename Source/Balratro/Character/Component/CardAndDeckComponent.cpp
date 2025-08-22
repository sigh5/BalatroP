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

#include "GameData/HandRankingStat.h"
#include "Interface/CalculatorScoreInterface.h"


void UCardAndDeckComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto VM = GetVMCardDeck();
	const auto VM_ItemSelcet = GetVMItemSelect();

	auto	PS = GetPlayerState();
	PS->OnSelectNextScene.AddUObject(this, &UCardAndDeckComponent::SetVisibleCardDeckView);

	VM->OnSortTypeChange.AddUObject(this, &UCardAndDeckComponent::SortHandInCard);
	VM->OnUseChuck.AddUObject(this, &UCardAndDeckComponent::UpdateChuck);
	VM->OnUseHandPlay.AddUObject(this, &UCardAndDeckComponent::UpdateHandPlay);
	

	VM_ItemSelcet->OnUseTaroCard.AddUObject(this, &UCardAndDeckComponent::UseTaroItem);
	//VM->OnSkipButtonClicked.AddUObject(this, &UCardAndDeckComponent::Test111);
	// 디버그용
	//InitDeck();
}

void UCardAndDeckComponent::UpdateCardInHand(TArray<FDeckCardStat>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	auto CurHandInCard = PS->GetCurrentAllHandsModify();
	CurHandInCard.RemoveAll([&](UHandInCard_Info* HandCard)
		{
			if (!HandCard)
				return false;

			return _DeckCardStat.ContainsByPredicate([&](const FDeckCardStat& Stat)
				{
					return Stat == HandCard->Info;
				});
		});
	// 나중에 DeckNumIndex ==> 카드가 덱에 추가된 순서를 csv에 넣어줘야함

	PS->SetCurrentAllHands(CurHandInCard);

	TArray<FDeckCardStat> Empty;
	PS->SetCurCalculatorCardInHands(Empty);

}

void UCardAndDeckComponent::SetVisibleCardDeckView(EPlayerStateType InValue)
{
	auto VM_MainMenu = GetVMMainWidget();
	auto VM_CardDeck = GetVMCardDeck();

	if (InValue == EPlayerStateType::SMALL_BLIND || InValue == EPlayerStateType::BIG_BLIND
		|| InValue == EPlayerStateType::SMALL_BLIND)
	{
		VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", true));
		VM_CardDeck->SetItemSelectFlag(false);
		InitDeck();
	}
	else if (InValue == EPlayerStateType::ITEM_SELECT)
	{
		VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", true));
		VM_CardDeck->SetItemSelectFlag(true);
		InitDeck();

		//UE_LOG(LogTemp, Warning, TEXT("SetVisibleCardDeckView"));
	}
	else
		return;
}

void UCardAndDeckComponent::FinishHandPlay()
{
	auto PS = GetPlayerState();
	
	int32 SumScore = PS->GetCurrentScore();
	int32 CurrentSum = ResultScore + SumScore;

	if (CurrentSum >= PS->GetCurrentRoundBlindGrade())
	{
		ResultScore = 0;
		PS->SetMaxScore(CurrentSum);
		PS->SetCurrentScore(CurrentSum);

		// 라운드 끝 로직 실행
		// 한 2초까지 점수 보여주고 그 뒤에 상점 뷰가게 만들어야함 
		GetWorld()->GetTimerManager().ClearTimer(TotalScoreHandle);

		auto VM_MainWidget = GetVMMainWidget();
		VM_MainWidget->SetCurWidgetName(FWidgetFlag_Info("CardDeckView", false));

		PS->SetPlayerState(EPlayerStateType::REWARD);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TotalScoreHandle);
		
		PS->SetCurrentScore(ResultScore);
		UpdateCardInHand(_CurData);
		DrawCard(_CardNum);
	}
}

void UCardAndDeckComponent::UseTaroItem(FTaroStat& TaroStat)
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();

	auto TaroTable = PS->GetTaroStatTableModify();

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
}

void UCardAndDeckComponent::UseEnhanceTaro(int32 EnhanceType)
{
	auto PS = GetPlayerState();
	auto& Cards = PS->GetCurCalculatorCardInHands();

	for (auto& Card : Cards)
	{
		if (EnhanceType >= 9)
		{
			if (static_cast<EnforceStatType>(EnhanceType) == EnforceStatType::CHANGE_HEART)
			{
				FName CurName = Card->Info.Name;
				FString NameStr = CurName.ToString();

				if (!NameStr.IsEmpty())
				{
					NameStr[0] = 'H';
				}

				Card->Info.Name = FName(*NameStr);
				Card->Info.SuitGrade = 3;
			}
			else if (static_cast<EnforceStatType>(EnhanceType) == EnforceStatType::CHANGE_SPADE)
			{
				FName CurName = Card->Info.Name;
				FString NameStr = CurName.ToString();

				if (!NameStr.IsEmpty())
				{
					NameStr[0] = 'D';
				}

				Card->Info.Name = FName(*NameStr);
				Card->Info.SuitGrade = 1;
			}

			
			FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Card/%s.%s"), *Card->Info.Name.ToString(), *Card->Info.Name.ToString());
			Card->Info.CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
			

		}
		else
		{
			Card->Info.EnforceType = static_cast<EnforceStatType>(EnhanceType);
		}
	}
}

void UCardAndDeckComponent::UseSealTaro(int32 SealType)
{
}

void UCardAndDeckComponent::UseGhostTaro(int32 GhostType)
{
}

void UCardAndDeckComponent::SetPlayCardEffect()
{
	auto PS = GetPlayerState();
	auto VM = GetVMCardDeck();
	
	auto CurPlayCards = PS->GetCurCalculatorCardInHands();
	
	TArray<FDeckCardStat> CurCardsInfo;

	for (auto Card : CurPlayCards)
	{
		FDeckCardStat Info;
		Info = Card->Info;
		CurCardsInfo.Add(Info);
	}


	VM->SetCurCardsData(CurCardsInfo);
	
	int32 CurCardNum = CurPlayCards.Num();
	int32 EnforceTypeNum = 0;
	int32 GhostTypeNum = 0;
	for (auto Card : CurPlayCards)
	{
		if (Card->Info.EnforceType != EnforceStatType::NONE)
		{
			EnforceTypeNum++;
		}

		if (Card->Info.GhostCardType != GhostCardStatType::NONE)
		{
			GhostTypeNum++;
		}
	}

	float CurDelayTime = (CurCardNum)+(EnforceTypeNum)+(GhostTypeNum);
	_DelayTime = CurDelayTime;

	// 여기서 이제 
	// 1번) 현재 패에 있는 배수 카드 계산,
	// 2번) 현재 손패에 남은 카드에서 스틸카드 계산
	// 3번) 조커 카드 계산
	// 할때까지 패 안뽑고 점수 계산하게 만들어야함
}

void UCardAndDeckComponent::ShuffleDeck()
{
	auto PS = GetPlayerState();

	auto& MyDeckStatTable = PS->GetDeckStatTableModify();

	FDateTime Now = FDateTime::UtcNow();
	int64 Milliseconds = Now.ToUnixTimestamp() * 1000 + Now.GetMillisecond();
	int32 Seed = static_cast<int32>(Milliseconds & 0xFFFFFFFF);
	FMath::RandInit(Seed);

	int32 NumElements = MyDeckStatTable.Num();

	UE_LOG(LogTemp, Log, TEXT("NumElements %d"), NumElements);

	for (int32 i = NumElements - 1; i > 0; --i)
	{
		int32 RandomIndex = FMath::RandRange(0, i);
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

	//VM->SetCurrentAllHands(PS->GetCurrentAllHands());
	VM->SetIsSelectedMax(false);

	PS->SetCardInDeckNum(PS->GetCardInDeckNum() - DrawCardNum);
	CurDrawIndex += DrawCardNum;
}

void UCardAndDeckComponent::UpdateChuck(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
{
	auto PS = GetPlayerState();

	int32 CurChuckCnt = PS->GetUseChuckCount();
	if (PS->GetMaxChuckCount() < CurChuckCnt + 1)
	{
		return;
	}

	PS->SetUseChuckCount(++CurChuckCnt);
	UpdateCardInHand(_DeckCardStat);
	DrawCard(CardNum);
}

void UCardAndDeckComponent::UpdateHandPlay(int32 CardNum, TArray<FDeckCardStat>& _DeckCardStat)
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

				_CurData = _DeckCardStat;
				_CardNum = CardNum;

				FTimerDelegate Delegate;
				Delegate.BindLambda([&]()
					{
						FinishHandPlay();
					});

				GetWorld()->GetTimerManager().SetTimer(TotalScoreHandle, Delegate, _DelayTime, false);
				break;
			}
		}
	}

	PS->SetUseHandCount(UseCount + 1);
}

void UCardAndDeckComponent::InitDeck()
{
	CurDrawIndex = 0;
	_CardNum = 0;
	_CurData.Empty();
	_DelayTime = 0.f;
	ResultScore = 0;

	ShuffleDeck();
	DrawCard(8);
}

void UCardAndDeckComponent::SortHandInCard(const EHandInCardSortType& InType)
{
	auto PS = GetPlayerState();
	auto& CurHandInCard = PS->GetCurrentAllHandsModify();
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
