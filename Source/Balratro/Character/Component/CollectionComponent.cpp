
#include "Character/Component/CollectionComponent.h"

#include <Engine/World.h>
#include <MVVMGameSubsystem.h>
#include <MVVMSubsystem.h>

#include "Singleton/BBGameSingleton.h"

#include "UI/MVVM/ViewModel/VM_Collection.h"
#include "UI/MVVM/ViewModel/VM_MainMenu.h"
#include "UI/MVVM/ViewModel/VM_JockerSlot.h"

#include "Core/MyPlayerState.h"


// Called when the game starts
void UCollectionComponent::BeginPlay()
{
	Super::BeginPlay();

	auto VM_Collection = GetVMCollection();
	auto VM_Joker = GetVMJockerSlot();
	auto PS = GetPlayerState();

	VM_Collection->OnShowCollection.AddUObject(this, &UCollectionComponent::ActiveCollectionView);
	VM_Joker->OnSetJokerState.AddUObject(this, &UCollectionComponent::AddJokerCollection);
	PS->OnBoucherCollection.AddUObject(this, &UCollectionComponent::AddBoucherCollection);

}

void UCollectionComponent::InitCollectionData()
{
	auto JokerData = UBBGameSingleton::Get().GetJokerStatTable();

	for (auto Joker : JokerData)
	{
		//JokerCollection.Add(*Joker, false);
	}

}

void UCollectionComponent::AddJokerCollection(FJokerStat& Data, bool IsAdd)
{
	if (IsAdd == false)
		return;

	/*for (auto Joker: JokerCollection)
	{
		if (Joker.Key.JokerType == Data.JokerType )
		{
			if (Joker.Value == false)
			{
				Joker.Value = true;
				break;
			}
			else
				return;
		}
	}*/
}

void UCollectionComponent::AddBoucherCollection(FBoucherInfo Data)
{
	for (auto Boucher : BoucherCollection)
	{
		if (Boucher.Key == Data.Type)
		{
			if (Boucher.Value == false)
			{
				Boucher.Value = true;
				break;
			}
			else
				return;
		}
	}
}

void UCollectionComponent::ActiveCollectionView()
{
	auto VM_Collection = GetVMCollection();
	auto VM_MainMenu = GetVMMainWidget();

	VM_Collection->SetBoucherCollection(BoucherCollection);
	//VM_Collection->SetJokerCollection(JokerCollection);

	// VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("HandRankingView", false));
	//VM_MainMenu->SetCurWidgetName(FWidgetFlag_Info("", true));
}

UVM_Collection* UCollectionComponent::GetVMCollection()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_HandRankingCount");
	Context.ContextClass = UVM_Collection::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_Collection>(Found);
}

AMyPlayerState* UCollectionComponent::GetPlayerState()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	auto PlayerState = Pawn->GetController()->GetPlayerState<AMyPlayerState>();
	return PlayerState;
}



UVM_MainMenu* UCollectionComponent::GetVMMainWidget()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_MainMenu");
	Context.ContextClass = UVM_MainMenu::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_MainMenu>(Found);
}

UVM_JockerSlot* UCollectionComponent::GetVMJockerSlot()
{
	const auto VMCollection = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>()->GetViewModelCollection();

	FMVVMViewModelContext Context;
	Context.ContextName = TEXT("VM_JockerSlot");
	Context.ContextClass = UVM_JockerSlot::StaticClass();

	const auto Found = VMCollection->FindViewModelInstance(Context);
	return Cast<UVM_JockerSlot>(Found);
}
