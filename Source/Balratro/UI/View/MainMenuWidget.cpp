// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/MainMenuWidget.h"

#include "Components/ProgressBar.h"
#include "Components\TextBlock.h"
#include "Components\Image.h"

#include "UI/MVVM/ViewModel/VM_MainMenu.h"


UMainMenuWidget::UMainMenuWidget()
{
	ViewModelClass = UVM_MainMenu::StaticClass();
	ViewModelName = TEXT("VM_MainMenu");
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto VMInst = TryGetViewModel<UVM_MainMenu>();
	VMInst->AddFieldValueChangedDelegate(UVM_MainMenu::FFieldNotificationClassDescriptor::CurrentHealth, 
		INotifyFieldValueChanged::FFieldValueChangedDelegate::CreateUObject(this, &UMainMenuWidget::OnFieldChanged));

}

void UMainMenuWidget::OnFieldChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{
	const auto VM = TryGetViewModel<UVM_MainMenu>();
	UE_LOG(LogTemp, Log, TEXT("CurrentHealth changed - new percent '%f' - current/max = %d/%d"), VM->GetHealthPercent(), VM->GetCurrentHealth(), VM->GetMaxHealth());
	HealthProgressBar->SetPercent(VM->GetHealthPercent());
	
	FText CurrentHealth = FText::AsNumber(VM->GetCurrentHealth());
	TestText->SetText(CurrentHealth);

	if (VM->GetCurrentHealth() % 2 == 0)
	{
		BackGroundImage->SetColorAndOpacity(FColor::Red);
	}
	else
	{
		BackGroundImage->SetColorAndOpacity(FColor::Green);
	}

}

void UMainMenuWidget::OnFieldTextChanged(UObject* Object, UE::FieldNotification::FFieldId FieldId)
{

}
