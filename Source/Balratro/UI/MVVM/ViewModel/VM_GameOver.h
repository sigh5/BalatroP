
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_GameOver.generated.h"

/**
 *
 */
DECLARE_MULTICAST_DELEGATE(FOnNewRunButtonEvent);


USTRUCT(BlueprintType)
struct FGameOverInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BestHandScore = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChunkCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EntiCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoundCount = 0;
};

UCLASS()
class BALRATRO_API UVM_GameOver : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnNewRunButtonEvent	OnNewRunButtonEvent;

public:
	const FName GetMostHandRankingName()  const { return MostHandRankingName; }
	void SetMostHandRankingName(FName InValue)
	{
		MostHandRankingName = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MostHandRankingName);
	}

	const int32 GetSeedName()  const { return SeedName; }
	void SetSeedName(int32 InValue)
	{
		SeedName = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(SeedName);
	}


	const FGameOverInfo GetGameOver_Infos()  const { return GameOver_Infos; }
	void SetGameOver_Infos(FGameOverInfo InValue)
	{
		GameOver_Infos = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GameOver_Infos);
	}


	void	SetNewRunButtonEvent() { OnNewRunButtonEvent.Broadcast(); }


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FName MostHandRankingName;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 SeedName;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FGameOverInfo GameOver_Infos;
};
