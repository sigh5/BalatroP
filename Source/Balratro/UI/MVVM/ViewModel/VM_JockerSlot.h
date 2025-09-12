#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameData/JokerStat.h"
#include "GameData/EnumDatas.h"
#include "VM_JockerSlot.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSetJokerState, FJokerStat&, bool /*JokerData , SellOrAdd*/);

DECLARE_MULTICAST_DELEGATE(FOnEffectUIViewFinish);


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnJokerSlotSwapData, UJokerCard_Info* , UJokerCard_Info* /*Sour Dest*/ );

/**
 * 
 */

UCLASS()
class BALRATRO_API UVM_JockerSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnSetJokerState OnSetJokerState;
	FOnEffectUIViewFinish OnEffectUIViewFinish;
	FOnJokerSlotSwapData OnJokerSlotSwapData;
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

	const bool GetCalculatorFlag() const { return CalculatorFlag; }
	void SetCalculatorFlag(bool _InValue) {
		CalculatorFlag = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CalculatorFlag);
	}

	const EJokerType GetPlayEventJoker() const { return PlayEventJoker; }
	void SetPlayEventJoker(EJokerType _InValue) {
		PlayEventJoker = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(PlayEventJoker);
	}

	const EJokerType GetRoundFinishEventJoker() const { return RoundFinishEventJoker; }
	void SetRoundFinishEventJoker(EJokerType _InValue) {
		RoundFinishEventJoker = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(RoundFinishEventJoker);
	}

	const bool GetJokerEventStopFlag() const { return JokerEventStopFlag; }
	void SetJokerEventStopFlag(bool _InValue) {
		JokerEventStopFlag = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(JokerEventStopFlag);
	}

///////////////////////////////////////////////////////////////////////////
	void	SetJokerState(FJokerStat& Data,bool IsAdd) { OnSetJokerState.Broadcast(Data, IsAdd); }// JokerList에 내 조커 추가하기

	void	All_EffectFinish() { OnEffectUIViewFinish.Broadcast(); }
	

	const   bool GetIsLastHandPlay() const { return IsLastHandPlay; }
	void	SetIsLastHandPlay(bool _InValue) { IsLastHandPlay = _InValue; }

	const   EPokerHand GetCurrentPokerHand() const { return CurrentPokerHand; }
	void	SetCurrentPokerHand(EPokerHand _InValue) { CurrentPokerHand = _InValue; }


	const   float GetAddtionalValue() const { return AddtionalValue; }
	void	SetAddtionalValue(float _InValue) { AddtionalValue = _InValue; }


	const EJokerType GetCopyJokerSetting() const { return CopyJokerSetting; }
	void SetCopyJokerSetting(EJokerType _InValue) {
		CopyJokerSetting = _InValue;}

	void SwapCardData(UJokerCard_Info* Source, UJokerCard_Info* SwapDest)
	{
		OnJokerSlotSwapData.Broadcast(Source, SwapDest);
	}


private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<class UJokerCard_Info*> JokerDatas;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool	CalculatorFlag = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EJokerType	PlayEventJoker = EJokerType::NONE;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	bool	JokerEventStopFlag = false;


	UPROPERTY()
	bool	IsLastHandPlay = false;

	UPROPERTY()
	EPokerHand	CurrentPokerHand;

	UPROPERTY()
	float		AddtionalValue;

	UPROPERTY()
	EJokerType	CopyJokerSetting = EJokerType::NONE;


	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	EJokerType	RoundFinishEventJoker = EJokerType::NONE;

};
