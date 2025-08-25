#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/JokerStat.h"
#include "VM_JockerSlot.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddJoker , FJokerStat&)

/**
 * 
 */

UCLASS()
class BALRATRO_API UVM_JockerSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnAddJoker OnAddJoker;

public:
	const TArray<UJokerCard_Info*>& GetJokerDatas() const
	{
		return JokerDatas;
	}
	

	void SetJokerDatas(const TArray<UJokerCard_Info*>& InValue)
	{
		JokerDatas = InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(JokerDatas);
	}
///////////////////////////////////////////////////////////////////////////
	void	SetAddJokerCard(FJokerStat& Data) { OnAddJoker.Broadcast(Data); }// JokerList에 내 조커 추가하기



private:
	// Array of members of the party
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<class UJokerCard_Info*> JokerDatas;


};
