// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_HandRankingCount.generated.h"


UCLASS(BlueprintType)
class UHandRanking_Info :public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FText Name;

	UPROPERTY(BlueprintReadOnly)
	int32 ImageIndex;

	UPROPERTY(BlueprintReadOnly)
	FGuid ID = FGuid();

	UPROPERTY(BlueprintReadOnly)
	int32 UseNum;
};


/**
 * 
 */
UCLASS()
class BALRATRO_API UVM_HandRankingCount : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	const TArray<UHandRanking_Info*>& GetHandRankingNum() const
	{
		return HandRankingNum;
	}

	void SetHandRankingNum(TArray<UHandRanking_Info*>& InHandRanking)
	{
		HandRankingNum = InHandRanking;
	}

	void AddHandRankingNum(const FText& Name)
	{
		auto HandRanking = NewObject<UHandRanking_Info>(this);
		HandRanking->Name = Name;
		HandRanking->ImageIndex = 0;
		HandRanking->UseNum = 0;

		HandRankingNum.Add(HandRanking);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HandRankingNum);
	}


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<UHandRanking_Info*> HandRankingNum;

};
