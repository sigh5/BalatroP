
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Item/BoosterPackData.h"
#include "GameData/TaroStat.h"
#include "GameData/DeckCardStat.h"
#include "VM_ItemSelect.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUseTaroCard, FTaroStat&);

/**
 *
 */

UCLASS()
class BALRATRO_API UVM_ItemSelect : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	FOnUseTaroCard OnUseTaroCard;

public:
	const TArray<FTaroStat> GetShowTaroInfo()  const { return ShowTaroInfo; }
	void SetShowTaroInfo(TArray<FTaroStat>& _InValue)
	{
		ShowTaroInfo = _InValue;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ShowTaroInfo);
	}

	const TArray<FDeckCardStat>& GetSelectCardInfos() const { return SelectCardInfos; }
	void SetSelectCardInfos(TArray<FDeckCardStat>& _InValue)  { SelectCardInfos.Empty(); SelectCardInfos = _InValue; }

	void	UseTaroItem(FTaroStat& TaroStat) { OnUseTaroCard.Broadcast(TaroStat); }

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	TArray<FTaroStat>   ShowTaroInfo;


	TArray<FDeckCardStat>   SelectCardInfos;

};
