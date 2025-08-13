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

	VMInst->AddFieldValueChangedDelegate(UVM_Reward::FFieldNotificationClassDescriptor::BlindImageIndex,
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

void URewardView::VM_FieldChanged_BlindRewardText(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 Reward = VMInst->GetBlindReward();
	if (Reward == -1)
		return;

	AnimStep = 0; // 시작
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
		{
			UpdateDollarAnimation(BlindReward, VMInst->GetBlindReward()); // MyValue를 함수에 전달
		});

	//GetWorld()->GetTimerManager().SetTimer(DollarAnimTimer, this, &URewardView::UpdateDollarAnimation, 0.2f, true);
	GetWorld()->GetTimerManager().SetTimer(DollarAnimTimer, TimerDel, 0.2f, true);
}

void URewardView::VM_FieldChanged_RestHands(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 RestHands = VMInst->GetRestHands();
	if (RestHands == -1)
		return;

	FString str ="";
	for (int i = 0; i < RestHands; ++i)
	{
		str += "$";
	}

	HandReward->SetText(FText::FromString(str));
	RestHand->SetText(FText::AsNumber(RestHands));
}

void URewardView::VM_FieldChanged_Interest(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 interest = VMInst->GetInterest();

	if (interest == -1)
		return;

	FString str = "";
	for (int i = 0; i < interest; ++i)
	{
		str += "$";
	}

	InterestReward->SetText(FText::FromString(str));
	Interrest->SetText(FText::AsNumber(interest));
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

	int32 ImageIndex = VMInst->GetBlindImageIndex();
	FString AssetPath = "";
	TSoftObjectPtr<UPaperSprite> MyAsset;
	if (ImageIndex == 0)
	{
		AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Blind/BlindChips_Sprite_0.BlindChips_Sprite_0"));
	}
	else if (ImageIndex == 1)
	{
		AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Blind/BlindChips_Sprite_21.BlindChips_Sprite_21"));
	}
	else if (ImageIndex == 2) // 상점 이미지
	{
		AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Shop/ShopSignAnimation_Sprite_3.ShopSignAnimation_Sprite_3"));
	}
	else if (ImageIndex == 3) /// 보스 블라인드 이미지가 달라서
	{

	}

	MyAsset = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
	if (MyAsset.IsValid())
	{
		MyAsset.LoadSynchronous();
	}

	UPaperSprite* Sprite = MyAsset.Get();
	FSlateBrush SpriteBrush;
	SpriteBrush.SetResourceObject(Sprite);
	SpriteBrush.ImageSize = FVector2D(100.f, 150.f);
	SpriteBrush.DrawAs = ESlateBrushDrawType::Image;
	BlindImage->SetBrush(SpriteBrush);
}

void URewardView::VM_FieldChanged_EarnGold(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_Reward>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 EarnGold = VMInst->GetEarnGold();
	
	FString str = FString::Printf(TEXT("Cash Out : $%d"),EarnGold);
	GoldText->SetText(FText::FromString(str));
}

void URewardView::UpdateDollarAnimation(UTextBlock* myText,int32 MaxNum)
{
	if (AnimStep >= MaxNum) // 1~3 반복
	{
		GetWorld()->GetTimerManager().ClearTimer(DollarAnimTimer);
		AnimStep = 0;
		return;
	}

	FString Text = BlindReward->GetText().ToString();
	for (int i = 0; i < 1; i++)
	{
		Text += TEXT("$");
	}
	myText->SetText(FText::FromString(Text));

	AnimStep++;
	
}
