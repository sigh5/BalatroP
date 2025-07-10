// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton/BBGameSingleton.h"

UBBGameSingleton::UBBGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/DT_HandRankingStatTable.DT_HandRankingStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);
	
		for (const auto& Row : DataTable->GetRowMap())
		{
			const FName RowName = Row.Key;
			FHandRankingStat* StatPtr = reinterpret_cast<FHandRankingStat*>(Row.Value);
			if (StatPtr)
			{
				// 기존 TMap<FName, FHandRankingStat*>에 추가
				HandRankingStatTable.Add(RowName, StatPtr);
			}
		}

		HandRankingMaxNum = HandRankingStatTable.Num();
		ensure(HandRankingMaxNum > 0);
	}
}

UBBGameSingleton& UBBGameSingleton::Get()
{
	UBBGameSingleton* singleton = CastChecked<UBBGameSingleton>(GEngine->GameSingleton);
	if (singleton)
	{
		return *singleton;
	}

	UE_LOG(LogTemp, Error, TEXT("Invalid Game Sigleton"));
	return *NewObject<UBBGameSingleton>();
}
