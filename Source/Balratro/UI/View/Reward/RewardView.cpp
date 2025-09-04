#include "UI/View/Reward/RewardView.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"
#include "UI/MVVM/ViewModel/VM_BlindSelect.h"
#include "UI/MVVM/ViewModel/VM_Reward.h"

#include "PaperSprite.h"
#include "Styling/SlateBrush.h"

URewardView::URewardView()
{
	ViewModelClass = UVM_Reward::StaticClass();
	ViewModelName = TEXT("VM_Reward");
}

void URewardView::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::BlindReward,
		FFieldValueChangedDelegate::CreateUObject(this, &URewardView::VM_FieldChanged_BlindRewardText));

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::RestHands,
		FFieldValueChangedDelegate::CreateUObject(this, &URewardView::VM_FieldChanged_RestHands));

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::Interest,
		FFieldValueChangedDelegate::CreateUObject(this, &URewardView::VM_FieldChanged_Interest));

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::BlindGrade,
		FFieldValueChangedDelegate::CreateUObject(this, &URewardView::VM_FieldChanged_BlindGrade));

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::BlindMaterialPath,
		FFieldValueChangedDelegate::CreateUObject(this, &URewardView::VM_FieldChanged_BlindImageIndex));

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::EarnGold,
		FFieldValueChangedDelegate::CreateUObject(this, &URewardView::VM_FieldChanged_EarnGold));
}

void URewardView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CashOutButton->OnClicked.AddDynamic(this, &URewardView::OnCashOutButton);
}

void URewardView::OnCashOutButton()
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->SetNextButtonClicked();
}

void URewardView::StartQueueAnimation()
{
	if (StartQueue.IsEmpty())
	{
		GoldText->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(GoldTextAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		return;
	}
	if (GetWorld()->GetTimerManager().IsTimerActive(DollarAnimTimer))
	{
		return;
	}
	else
	{
		FTimerDelegate CurDelegator;
		StartQueue.Peek(CurDelegator);
		GetWorld()->GetTimerManager().SetTimer(DollarAnimTimer, CurDelegator, 0.3f, true);

		StartQueue.Pop();
	}	
}

void URewardView::VM_FieldChanged_BlindRewardText(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	RewardStep = VMInst->GetBlindReward();
	if (RewardStep == -1)
		return;
	BlindReward->SetText(FText::FromString(""));


	AnimStep = 0; // 시작
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
		{
			UpdateDollarAnimation(nullptr, BlindReward, RewardStep);
		});

	StartQueue.Enqueue(TimerDel);
}

void URewardView::VM_FieldChanged_RestHands(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	RestHandStep = VMInst->GetRestHands();
	if (RestHandStep == -1)
		return;
	RestHand->SetText(FText::FromString(""));
	HandReward->SetText(FText::FromString(""));

	AnimStep = 0;

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
		{
			UpdateDollarAnimation(RestHand,HandReward, RestHandStep);
		});

	StartQueue.Enqueue(TimerDel);

	UE_LOG(LogTemp, Warning, TEXT("VM_FieldChanged_RestHands"));
}

void URewardView::VM_FieldChanged_Interest(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	interestStep = VMInst->GetInterest();

	if (interestStep == -1)
		return;
	
	Interrest->SetText(FText::FromString(""));
	InterestReward->SetText(FText::FromString(""));
	
	AnimStep = 0;

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
		{
			UpdateDollarAnimation(Interrest,InterestReward, interestStep);
		});

	StartQueue.Enqueue(TimerDel);

	StartQueueAnimation();
}

void URewardView::VM_FieldChanged_BlindGrade(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 Grade = VMInst->GetBlindGrade();
	FNumberFormattingOptions NumberFormatOptions;
	BlindGradeText->SetText(FText::AsNumber(Grade, &NumberFormatOptions));
}

void URewardView::VM_FieldChanged_BlindImageIndex(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	FString MaterialPath = VMInst->GetBlindMaterialPath();
	
	FStringAssetReference MatRef = MaterialPath;
	UMaterialInterface* LoadedMat = Cast<UMaterialInterface>(MatRef.TryLoad());
	if (LoadedMat)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(LoadedMat);
		BlindImage->SetBrush(Brush);
	}
}

void URewardView::VM_FieldChanged_EarnGold(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 EarnGold = VMInst->GetEarnGold();
	
	FString str = FString::Printf(TEXT("Cash Out : $%d"),EarnGold);
	GoldText->SetText(FText::FromString(str));

	GoldText->SetVisibility(ESlateVisibility::Collapsed);
}

void URewardView::UpdateDollarAnimation(class UTextBlock* numberText, class UTextBlock* strText, int32 MaxNum)
{
	if (AnimStep >= MaxNum) // 1~3 반복
	{
		GetWorld()->GetTimerManager().ClearTimer(DollarAnimTimer);
		AnimStep = 0;
		
		if (numberText)
		{
			numberText->SetText(FText::AsNumber(MaxNum));
		}

		StartQueueAnimation();
		return;
	}

	FString Text = strText->GetText().ToString();
	for (int i = 0; i < 1; i++)
	{
		Text += TEXT("$");
	}
	strText->SetText(FText::FromString(Text));

	AnimStep++;
	
}
