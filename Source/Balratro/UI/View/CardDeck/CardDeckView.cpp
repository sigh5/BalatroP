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

#include "UI/Button/Card/CardButton.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_CardDeck.h"

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

	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurrentAllHands,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_HandInCard));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::IsUpCardExist,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CardUpExist));

	VMInst->AddFieldValueChangedDelegate(UVM_CardDeck::FFieldNotificationClassDescriptor::CurCardsData,
		FFieldValueChangedDelegate::CreateUObject(this, &UCardDeckView::VM_FieldChanged_CurPlayCardData));
}

void UCardDeckView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SuitSortButton->OnClicked.AddDynamic(this, &UCardDeckView::OnSuitSortButtonClicked);
	RankSortButton->OnClicked.AddDynamic(this, &UCardDeckView::OnRankSortButtonClicked);
	HandPlayButton->OnClicked.AddDynamic(this, &UCardDeckView::OnHandPlayButtonClicked);
	ChuckButton->OnClicked.AddDynamic(this, &UCardDeckView::OnChuckButtonClicked);
}

UCardButton* UCardDeckView::ReuseCardButton(int32 CurAllCardNum ,int32 CurNum, UHandInCard_Info* CardInfo)
{
	UCardButton* NewButton = nullptr;
	if (HandCardButton.Num() < CurAllCardNum)
	{
		NewButton = NewObject<UCardButton>(this);
		NewButton->SetCardInfoData(CardInfo->Info);
		NewButton->SetCardIndex(CardIndex++);
		HandCardButton.Add(NewButton);
	}
	else
	{
		NewButton = HandCardButton[CurNum];
		check(NewButton);
		NewButton->SetSelected(false);
		NewButton->SetCardInfoData(CardInfo->Info);
		NewButton->SetCardIndex(CardIndex++);
	}
	return NewButton;
}

void UCardDeckView::VM_FieldChanged_HandInCard(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInstance = Cast<UVM_CardDeck>(Object);
	auto& CurHandInfo = VMInstance->GetCurrentAllHands();
	int32 CurAllHandNum = CurHandInfo.Num();
	
	CardPanel->ClearChildren(); // 기존 이미지 제거

	for (int i = 0; i < CurAllHandNum; ++i)
	{
		UCardButton* NewButton = ReuseCardButton(CurAllHandNum, i, CurHandInfo[i]);

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

	TArray<FDeckCardStat> CardStatInfo;
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
	{
		if (SelectedNum == 0)
		{
			VMInst->BrodCastrHandRankName(0, CardStatInfo);
		}
	}

	VMInst->BrodCastrHandRankName(SelectedNum, CardStatInfo);
}

void UCardDeckView::VM_FieldChanged_CurPlayCardData(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	auto& Data = VMInst->GetCurCardsData();
	
	CurPlayCardNum = 0;
	
	ChipText->SetVisibility(ESlateVisibility::HitTestInvisible);

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
		{
			CardChipScoreText();
		});
	
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, TimerDelegate, 0.5f, true, 0.f);
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
	
	TArray<FDeckCardStat> CardStatInfo; 
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
		return;
	
	VMInst->UseChuck(SelectedNum, CardStatInfo);
}

void UCardDeckView::OnHandPlayButtonClicked()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();

	TArray<FDeckCardStat> CardStatInfo;
	int32 SelectedNum = 0;

	if (SetCardData(CardStatInfo, SelectedNum) == false)
		return;

	VMInst->UseHandPlay(SelectedNum, CardStatInfo);
}

void UCardDeckView::CardChipScoreText()
{
	const auto VMInst = TryGetViewModel<UVM_CardDeck>();
	auto& Data = VMInst->GetCurCardsData();

	if (CurPlayCardNum >= Data.Num())
	{
		ChipText->SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(MyTimerHandle);
	}
	else
	{
		int32 ChipGrade = Data[CurPlayCardNum].BaseChip;
		int32 DraiageGrade = 0;

		EnforceStatType Type = Data[CurPlayCardNum].EnforceType;
		switch (Type)
		{
		case EnforceStatType::NONE:
			break;
		case EnforceStatType::CHIP_PLUS:
			ChipGrade += 40;
			break;
		case EnforceStatType::DRAINAGE:
			DraiageGrade += 4;
			break;
		case EnforceStatType::STEEL:
			break;
		case EnforceStatType::GOLD:
			break;
		case EnforceStatType::GLASS:
			break;
		default:
			break;
		}

		UCardButton* CurCardButton = nullptr;
		for (auto& Card : HandCardButton)
		{
			if (Card->GetCardInfoData() == Data[CurPlayCardNum])
			{
				CurCardButton = Card;
				break;
			}
		}
		
		if (CurCardButton)
		{
			ChipText->SetText(FText::AsNumber(Data[CurPlayCardNum].BaseChip));
			
			auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>(TEXT("VM_PlayerInfo"), UVM_PlayerInfo::StaticClass());
			check(VM_PlayerInfo);

			int32 CurChip =  VM_PlayerInfo->GetCurChip();
			CurChip += Data[CurPlayCardNum].BaseChip;
			VM_PlayerInfo->SetCurChip(CurChip);


			FGeometry CardGeo = CurCardButton->GetCachedGeometry();
			FVector2D AbsPos = CardGeo.GetAbsolutePosition();
			FVector2D Size = CardGeo.GetLocalSize();

			if (UWidget* CanvasParent = ChipText->GetParent())
			{
				FGeometry ParentGeo = CanvasParent->GetCachedGeometry();
				FVector2D LocalCenter = ParentGeo.AbsoluteToLocal(AbsPos);
				
				LocalCenter.Y -= 55.f;
				LocalCenter.X -= (Size.X * 0.39f);
				
				if (UCanvasPanelSlot* ChipSlot = Cast<UCanvasPanelSlot>(ChipText->Slot))
				{
					ChipSlot->SetPosition(LocalCenter);
				}
			}
		}
	}
	++CurPlayCardNum;
}

bool UCardDeckView::SetCardData(OUT TArray<FDeckCardStat>& CardStatInfo, OUT int32& SelectedCardNum)
{
	for (auto& Button : HandCardButton)
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

