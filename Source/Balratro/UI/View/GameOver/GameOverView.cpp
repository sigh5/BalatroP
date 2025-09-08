#include "UI/View/GameOver/GameOverView.h"

#include "UI/MVVM/ViewModel/VM_GameOver.h"
#include "UI/MVVM/ViewModel/VM_PlayerInfo.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"


UGameOverView::UGameOverView()
{
	ViewModelClass = UVM_GameOver::StaticClass();
	ViewModelName = "VM_GameOver";
}

void UGameOverView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameOverView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto VMInst = TryGetViewModel<UVM_GameOver>();
	checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->AddFieldValueChangedDelegate(UVM_GameOver::FFieldNotificationClassDescriptor::GameOver_Infos,
		FFieldValueChangedDelegate::CreateUObject(this, &UGameOverView::VM_FieldChanged_GameOver_INT_Data));

	VMInst->AddFieldValueChangedDelegate(UVM_GameOver::FFieldNotificationClassDescriptor::MostHandRankingName,
		FFieldValueChangedDelegate::CreateUObject(this, &UGameOverView::VM_FieldChanged_MostHandRankingName));

	VMInst->AddFieldValueChangedDelegate(UVM_GameOver::FFieldNotificationClassDescriptor::SeedName,
		FFieldValueChangedDelegate::CreateUObject(this, &UGameOverView::VM_FieldChanged_SeedName));

	NewRunButton->OnClicked.AddDynamic(this, &UGameOverView::OnClicked_NewRunButton);
	MainMenuButton->OnClicked.AddDynamic(this, &UGameOverView::OnClicked_MainMenuButton);
}

void UGameOverView::VM_FieldChanged_GameOver_INT_Data(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_GameOver>(); checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	auto CurIntData = VMInst->GetGameOver_Infos();

	BestHandText->SetText(FText::AsNumber(CurIntData.BestHandScore));
	PlayNumText->SetText(FText::AsNumber(CurIntData.PlayCount));
	ChuckNumText->SetText(FText::AsNumber(CurIntData.ChunkCount));
	EntiNumText->SetText(FText::AsNumber(CurIntData.EntiCount));
	RoundNumText->SetText(FText::AsNumber(CurIntData.RoundCount));

	auto VM_PlayerInfo = TryGetViewModel<UVM_PlayerInfo>("VM_PlayerInfo", UVM_PlayerInfo::StaticClass()); check(VM_PlayerInfo);
	FString MaterialPath = VM_PlayerInfo->GetBlindMaterialPath();

	FStringAssetReference MatRef = MaterialPath;
	UMaterialInterface* LoadedMat = Cast<UMaterialInterface>(MatRef.TryLoad());
	if (LoadedMat)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(LoadedMat);
		DefeatImage->SetBrush(Brush);
	}
}

void UGameOverView::VM_FieldChanged_MostHandRankingName(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_GameOver>(); checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	FName CurName = VMInst->GetMostHandRankingName();

	MostHandRankingText->SetText(FText::FromName(CurName));

}

void UGameOverView::VM_FieldChanged_SeedName(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VMInst = TryGetViewModel<UVM_GameOver>(); checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	int32 CurName = VMInst->GetSeedName();

	SeedText->SetText(FText::AsNumber(CurName));
}

void UGameOverView::OnClicked_NewRunButton()
{
	const auto VMInst = TryGetViewModel<UVM_GameOver>(); checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->SetGameOverButtonEvent(EPlayerStateType::BLINDSELECT);
}

void UGameOverView::OnClicked_MainMenuButton()
{
	const auto VMInst = TryGetViewModel<UVM_GameOver>(); checkf(IsValid(VMInst), TEXT("Couldn't find a valid ViewModel"));

	VMInst->SetGameOverButtonEvent(EPlayerStateType::LOGO);
}
