#include "UI/View/CardDeck/CardDeckView.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"
#include "UI/MVVM/ViewModel/VM_ItemSelect.h"
#include "UI/Button/Card/CardButtonWidget.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "TimerManager.h"




UCardDeckView::UCardDeckView()
{
	ViewModelClass = UVM_CardDeck::StaticClass();
	ViewModelName = TEXT("VM_CardDeck");
}

void UCardDeckView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCardDeckView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CardButtonSubClass = LoadClass<UCardButtonWidget>(nullptr, TEXT("/Game/UI/View/CardDeck/WBP_CardButtonWidget.WBP_CardButtonWidget_C"));

	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurrentAllHands,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_HandInCard));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::IsUpCardExist,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CardUpExist));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurCardsData,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CurPlayCardData));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::ItemSelectFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_ItemSelectFlag));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::RestCardEffectFlag,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_RestCardData));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::BossSkillUse,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CurBossUseSkill));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurrentBossType,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CurBossText));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::Useless_EmblemType,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_UselessBlindEmblem));


	SuitSortButton->OnClicked.AddDynamic(this, &UCardDeckView::OnSuitSortButtonClicked);
	RankSortButton->OnClicked.AddDynamic(this, &UCardDeckView::OnRankSortButtonClicked);
	HandPlayButton->OnClicked.AddDynamic(this, &UCardDeckView::OnHandPlayButtonClicked);
	ChuckButton->OnClicked.AddDynamic(this, &UCardDeckView::OnChuckButtonClicked);

	BossSkillText->SetVisibility(ESlateVisibility::Collapsed);
}

UCardButtonWidget* UCardDeckView::ReuseCardButtonWidget(int32 CurAllCardNum, int32 CurNum, UHandInCard_Info* CardInfo)
{
	UCardButtonWidget* NewButton = nullptr;
	if (HandCardButtons.Num() < CurAllCardNum)
	{
		NewButton = CreateWidget<UCardButtonWidget>(this, CardButtonSubClass);
		HandCardButtons.Add(NewButton);
	}
	else
	{
		NewButton = HandCardButtons[CurNum];
		NewButton->SetSelected(false);
	}

	check(NewButton);
	
	NewButton->SetInfo(CardInfo);
	NewButton->SetCardIndex(CardIndex++);
	return NewButton;
}



void UCardDeckView::VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	GetWorld()->GetTimerManager().ClearTimer(FinishScoreTimerHandle);
	TimerFuncQueue.Empty();

	const auto VMInstance = Cast<UVM_CardDeck>(Object);
	auto& CurHandInfo = VMInstance->GetCurrentAllHands();
	int32 CurAllHandNum = CurHandInfo.Num();
	
	if (VMInstance->GetBossSkillUse())
	{
		BossSkillText->SetRenderOpacity(1.f);
	}

	//CardPanel->ClearChildren(); // 기존 이미지 제거

	for (int i = 0; i < CurAllHandNum; ++i)
	{
		UCardButtonWidget* NewButton = ReuseCardButtonWidget(CurAllHandNum, i, CurHandInfo[i]);
		UHorizontalBoxSlot* BoxSlot = CardPanel->AddChildToHorizontalBox(NewButton);
		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));

		FMargin Padding0;
		if (i == 0)
			Padding0 = FMargin(PaddingX * 2, 0.f, PaddingX, 0.f);
		else
			Padding0 = FMargin(PaddingX, 0.f, PaddingX, 0.f);
		
		BoxSlot->SetPadding(Padding0);
		BoxSlot->SetHorizontalAlignment(HAlign_Center);
		BoxSlot->SetVerticalAlignment(VAlign_Center);
	}
}

void UCardDeckView::VM_FieldChanged_CardUpExist(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	TArray<UHandInCard_Info*> CardStatInfo; // 전체 선택한 것
	int32 SelectedNum = 0;
	
	SetCardData(CardStatInfo, SelectedNum);

	VMInst->BroadCastCurHands(SelectedNum, CardStatInfo);
	
	const auto VM_ItemSelct = TryGetViewModel<UVM_ItemSelect>("VM_ItemSelect", UVM_ItemSelect::StaticClass());
	checkf(IsValid(VM_ItemSelct), TEXT("Couldn't find a valid ViewModel"));

	VM_ItemSelct->SetSelectCardInfos(CardStatInfo);
}

void UCardDeckView::VM_FieldChanged_CurPlayCardData(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	CardScroe_EffectText();
}

void UCardDeckView::VM_FieldChanged_ItemSelectFlag(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	if (VMInst->GetItemSelectFlag())
	{
		SuitSortButton->SetVisibility(ESlateVisibility::Collapsed);
		RankSortButton->SetVisibility(ESlateVisibility::Collapsed);
		HandPlayButton->SetVisibility(ESlateVisibility::Collapsed);

		ChuckButtonText->SetText(FText::FromString(TEXT("Skip")));
		HandSortText->SetText(FText::FromString(TEXT("Select2"))); // 나중에 숫자 필요
	}
	else
	{
		SuitSortButton->SetVisibility(ESlateVisibility::Visible);
		RankSortButton->SetVisibility(ESlateVisibility::Visible);
		HandPlayButton->SetVisibility(ESlateVisibility::Visible);
		
		ChuckButtonText->SetText(FText::FromString(TEXT("Chuck")));
		HandSortText->SetText(FText::FromString(TEXT("HandSort")));
	}
	
}

void UCardDeckView::VM_FieldChanged_RestCardData(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>(); check(VMInst);

	auto Data = VMInst->GetRestCardDatas();

	int32 UseLess_EmblemType = VMInst->GetUseless_EmblemType();

	for (int i = 0; i < Data.Num(); ++i)
	{
		for (auto& Card : HandCardButtons)
		{
			if (Card->GetCardInfoData()->Info.EnforceType == EnforceStatType::STEEL 
				&& Card->GetCardInfoData()->Info == Data[i]->Info && 
				UseLess_EmblemType != Card->GetCardInfoData()->Info.SuitGrade)
			{
				FDeckCardStat CurData = Data[i]->Info;
				SetRestCard_EffectOrder(Card, CurData);
				break;
			}
		}
	}

	StartNextTimer();
}

void UCardDeckView::VM_FieldChanged_CurBossUseSkill(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>(); check(VMInst);

	auto BossType = VMInst->GetCurrentBossType();

	BossSkillText->SetRenderOpacity(0.f);


	if (BossType == EBossType::HOOK)
	{
		TArray<UHandInCard_Info*>  CurAllCards = VMInst->GetCurrentAllHands();
		TArray<UHandInCard_Info*>  CurRestCards = VMInst->GetRestCardDatas();

		TArray<UHandInCard_Info*> CardStatInfo; // 전체 선택한 것
		int32 SelectedNum = 0;
		SetCardData(CardStatInfo, SelectedNum);

		for (auto& Card : HandCardButtons)
		{
			if (CurRestCards.Contains(Card->GetCardInfoData()))
				continue;

			if (CardStatInfo.Contains(Card->GetCardInfoData()))
				continue;

			SetCard_PrevEffectOrder(Card, Card->GetCardInfoData()->Info);
		}
	}
	else if (BossType == EBossType::OX)
	{
		FDeckCardStat None;
		None.Name = "OX";

		SetCard_PrevEffectOrder(nullptr, None);		
	}
	else if (BossType == EBossType::ARM)
	{
		FDeckCardStat None;
		None.Name = "ARM";

		SetCard_PrevEffectOrder(nullptr, None);
	}
}

void UCardDeckView::VM_FieldChanged_CurBossText(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>(); check(VMInst);

	auto CurBossType = VMInst->GetCurrentBossType();

	FText BossSkilltxt = FText(FText::FromString(GetBossSkillText(CurBossType)));
	BossSkillText->SetVisibility(ESlateVisibility::Visible);
	BossSkillText->SetRenderOpacity(1.f);
	BossSkillText->SetText(BossSkilltxt);
}

void UCardDeckView::VM_FieldChanged_UselessBlindEmblem(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>(); check(VMInst);

	int32 UseLessEmblem = VMInst->GetUseless_EmblemType();

	for (auto Card : HandCardButtons)
	{
		Card->SetGoadEvent();
	}
}

void UCardDeckView::OnSuitSortButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	VMInst->SetSuitSort();
}

void UCardDeckView::OnRankSortButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	VMInst->SetRankSort();
}

void UCardDeckView::OnChuckButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	
	if (VMInst->GetItemSelectFlag())
	{
		// SKip 액션 실행 (저번 씬 창으로 이동해야됌)
		VMInst->SkipButtonClicked();
	}
	else
	{
		TArray<UHandInCard_Info*> CardStatInfo;
		int32 SelectedNum = 0;

		if (SetCardData(CardStatInfo, SelectedNum) == false)
			return;

			VMInst->UseChuck(SelectedNum, CardStatInfo);
	}
}

void UCardDeckView::OnHandPlayButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();

	TArray<UHandInCard_Info*>  CardStatInfo;
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
		return;

	VMInst->UseHandPlay(SelectedNum, CardStatInfo);
}

void UCardDeckView::CardScroe_EffectText()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	auto& Data = VMInst->GetCurCardsData();
	
	int32 UseLess_EmblemType = VMInst->GetUseless_EmblemType();

	for (int i = 0; i < Data.Num(); ++i)
	{
		for (auto& Card : HandCardButtons)
		{
			if (Card->GetCardInfoData()->Info == Data[i]->Info &&
				UseLess_EmblemType != Card->GetCardInfoData()->Info.SuitGrade)
			{
				FDeckCardStat CurData = Data[i]->Info;
				Card->MoveAnimmation();
				SetCard_EffectOrder(Card, Data[i]->Info);
				break;
			}
		}
	}
}

void UCardDeckView::SetCard_PrevEffectOrder(UCardButtonWidget* EventCard, FDeckCardStat& CardData)
{
	if (EventCard)
	{
		PushTimerEvent([&](UCardButtonWidget* CurEventCard, int32 Value)
			{
				CurEventCard->ChuckAnimation();
			}, EventCard, CardData.BaseChip);
	}
	else
	{
		if (CardData.Name == "OX" || CardData.Name == "ARM")
		{
			PushTimerEvent([&](UCardButtonWidget* CurEventCard, int32 Value)
				{
					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>("VM_PlayerInfo", UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					VM_PlayerInfo->RefreshPlayerInfoViewEvent();
				}, nullptr, 0);
		}
	}

}

void UCardDeckView::SetCard_EffectOrder(UCardButtonWidget* EventCard, FDeckCardStat& CardData)
{
	if (EventCard)
	{
		PushTimerEvent([&](UCardButtonWidget* CurEventCard, int32 Value)
		{

			ScoreResultText->SetVisibility(ESlateVisibility::HitTestInvisible);

			auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
			check(VM_PlayerInfo);

			int32 CurChip = VM_PlayerInfo->GetCurChip();
			CurChip += Value;
			VM_PlayerInfo->SetCurChip(CurChip);

			FString ScoreStr = FString::Printf(TEXT("+%d"), Value);
			ScoreResultText->SetText(FText::FromString(ScoreStr));
			SetScoreTextPos(CurEventCard,false);
		}, EventCard, CardData.BaseChip);

		
		if (CardData.EnforceType == EnforceStatType::DRAINAGE)
		{
			PushTimerEvent([&](UCardButtonWidget* CurEventCard, int32 Value)
				{
					ScoreResultText->SetVisibility(ESlateVisibility::HitTestInvisible);

					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					int32 CurDriange = VM_PlayerInfo->GetCurDrainage();
					CurDriange += Value;

					VM_PlayerInfo->SetCurDrainage(CurDriange);

					FString ScoreStr = FString::Printf(TEXT("+%d Drainage"), Value);
					ScoreResultText->SetText(FText::FromString(ScoreStr));

					SetScoreTextPos(CurEventCard,false);
				}, EventCard, 4);
		}
		else if (CardData.EnforceType == EnforceStatType::CHIP_PLUS)
		{
			PushTimerEvent([&](UCardButtonWidget* CurEventCard, int32 Value)
				{
					ScoreResultText->SetVisibility(ESlateVisibility::HitTestInvisible);

					auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
					check(VM_PlayerInfo);

					int32 CurChip = VM_PlayerInfo->GetCurChip();
					CurChip += Value;
					VM_PlayerInfo->SetCurChip(CurChip);

					FString ScoreStr = FString::Printf(TEXT("+%d"), Value);
					ScoreResultText->SetText(FText::FromString(ScoreStr));
					SetScoreTextPos(CurEventCard,false);

				}, EventCard, 30);
		}
	
	}
}

void UCardDeckView::SetRestCard_EffectOrder(UCardButtonWidget* EventCard, FDeckCardStat& CardData)
{
	if (CardData.EnforceType == EnforceStatType::STEEL)
	{
		PushTimerEvent([&](UCardButtonWidget* CurEventCard, int32 Value)
			{
				ScoreResultText->SetVisibility(ESlateVisibility::HitTestInvisible);

				CurEventCard->ShakeAnimation();

				auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
				check(VM_PlayerInfo);

				float CurDrainage = VM_PlayerInfo->GetCurDrainage();
				CurDrainage *= 1.5f;
				VM_PlayerInfo->SetCurDrainage(CurDrainage);

				FString ScoreStr = FString::Printf(TEXT("x 1.5"));
				ScoreResultText->SetText(FText::FromString(ScoreStr));
				SetScoreTextPos(CurEventCard, true);

			}, EventCard, 30);
	}
}

void UCardDeckView::SetScoreTextPos(UCardButtonWidget* CurEventCard, bool IsUp)
{
	FGeometry CardGeo = CurEventCard->GetCachedGeometry();
	FVector2D AbsPos = CardGeo.GetAbsolutePosition();
	FVector2D Size = CardGeo.GetLocalSize();

	if (UWidget* CanvasParent = ScoreResultText->GetParent())
	{
		ScoreResultText->SetVisibility(ESlateVisibility::HitTestInvisible);
		FGeometry ParentGeo = CanvasParent->GetCachedGeometry();
		FVector2D LocalCenter = ParentGeo.AbsoluteToLocal(AbsPos);

		if (IsUp)
		{
			LocalCenter.Y -= 40.f;
		}
		else
		{
			LocalCenter.Y += 15.f;
		}
		LocalCenter.X -= (Size.X * 0.39f);

		if (UCanvasPanelSlot* ChipSlot = Cast<UCanvasPanelSlot>(ScoreResultText->Slot))
		{
			ChipSlot->SetPosition(LocalCenter);
		}
	}
}

void UCardDeckView::StartNextTimer()
{
	auto VM_Inst = TryGetViewModel<UVM_CardDeck>(); check(VM_Inst);

	if (VM_Inst->GetIsHandPlayFlag() == false)
	{
		TimerFuncQueue.Empty();
	}


	if(TimerFuncQueue.IsEmpty())
	{
		GetWorld()->GetTimerManager().ClearTimer(MyTimerHandle);

		FTimerDelegate ScoreTimerFinishDelegate; 
		ScoreTimerFinishDelegate.BindLambda([&]() {
			ScoreResultText->SetVisibility(ESlateVisibility::Collapsed);
			ScoreResultText->SetText(FText::FromString("")); 
			});

		GetWorld()->GetTimerManager().SetTimer(
			FinishScoreTimerHandle,
			ScoreTimerFinishDelegate,
			0.5f,
			false);

		auto VM_Joker = TryGetViewModel<UVM_JockerSlot>("VM_JockerSlot", UVM_JockerSlot::StaticClass()); check(VM_Joker);
		VM_Joker->SetCalculatorFlag(true);

		return;
	}
	
	FTimerDelegate Delegate;
	TimerFuncQueue.Dequeue(Delegate);

	GetWorld()->GetTimerManager().SetTimer(
		MyTimerHandle,
		Delegate,
		0.7f,
		false);
}

void UCardDeckView::PushTimerEvent(TFunction<void(UCardButtonWidget* , int32)> InFunc, UCardButtonWidget* CurEventCard,int32 InValue)
{
	FTimerDelegate Delegate;
	Delegate.BindLambda([this, InFunc, CurEventCard,InValue]()
		{
			// InValue를 그대로 전달해 실행
			InFunc(CurEventCard,InValue);
			// 다음 이벤트 실행
			StartNextTimer();
		});
	TimerFuncQueue.Enqueue(Delegate);
}

bool UCardDeckView::SetCardData(OUT TArray<UHandInCard_Info*>& CardStatInfo, OUT int32& SelectedCardNum)
{
	for (auto& Button : HandCardButtons)
	{
		if (Button->GetSelected())
		{
			++SelectedCardNum;
			CardStatInfo.Add(Button->GetCardInfoData());
		}
	}
	
	if (SelectedCardNum == 0)
		return false;

	return true;
}

FString UCardDeckView::GetBossSkillText(EBossType _InType)
{
	FString CurMessage = "";
	switch (_InType)
	{
	case EBossType::HOOK:
		CurMessage = TEXT("Discard 2 cards when playing");
		break;
	case EBossType::OX:
		CurMessage = TEXT("If you play the most played hand, your Gold is $0");
		break;
	case EBossType::WALL:
		CurMessage = TEXT("Extra-large blinds. twice grade ");
		break;
	case EBossType::ARM:
		CurMessage = TEXT("The level of the genealogy you played will be lowered");
		break;
	case EBossType::PSYCHIC:
		CurMessage = TEXT("You must play 5 cards");
		break;
	case EBossType::GOAD:
		CurMessage = TEXT("All spade cards are debuffed");
		break;
	case EBossType::WATER:
		CurMessage = TEXT("Start with 0 discards");
		break;
	case EBossType::EYE:
		CurMessage = TEXT("You can only play each hand type once in this Round");
		break;
	case EBossType::FINAL:
		CurMessage = TEXT("FinalRound");
		break;
	default:
		break;
	}

	return CurMessage;
}