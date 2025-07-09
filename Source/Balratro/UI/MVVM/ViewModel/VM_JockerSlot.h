#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_JockerSlot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UPartJoker : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FText Name;

	// Random generated ID
	UPROPERTY(BlueprintReadOnly)
	FGuid ID = FGuid();
};

UENUM()
enum class EPartyStatus : uint8
{
	Ready,
	NotReady,
};

UCLASS()
class BALRATRO_API UVM_JockerSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	const TArray<UPartJoker*>& GetPlayers() const
	{
		return Players;
	}
	

	void SetPlayers(const TArray<UPartJoker*>& InPlayers)
	{
		Players = InPlayers;
	}

	void AddPlayer(const FText& Name)
	{
		auto Player = NewObject<UPartJoker>(this);
		Player->Name = Name;
		Players.Add(Player);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Players);
	}

	void RemovePlayer(int Index)
	{
		Players.RemoveAt(Index);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Players);
	}

	EPartyStatus GetStatus() const
	{
		return Status;
	}

	void SetStatus(EPartyStatus InStatus)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Status, InStatus);
	}

private:
	// Array of members of the party
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UPartJoker*> Players;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EPartyStatus Status;
};
