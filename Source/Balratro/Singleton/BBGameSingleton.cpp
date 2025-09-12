// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton/BBGameSingleton.h"

#include "PaperSprite.h"

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


	static ConstructorHelpers::FObjectFinder<UDataTable> DeckDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/DT_DeckCardStat.DT_DeckCardStat'"));
	if (nullptr != DeckDataTableRef.Object)
	{
		const UDataTable* DataTable = DeckDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		for (const auto& Row : DataTable->GetRowMap())
		{
			const FName RowName = Row.Key;
			FDeckCardStat* StatPtr = reinterpret_cast<FDeckCardStat*>(Row.Value);
			if (StatPtr)
			{
				StatPtr->Name = RowName;
				FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Card/%s.%s"), *RowName.ToString(), *RowName.ToString());		
				StatPtr->CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
				
				AssetPath = TEXT("/Game/CardResuorce/CardEnfoceImage/EnforceSprite_0.EnforceSprite_0");
				StatPtr->EnforceSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));

				if(!StatPtr->EnforceSprite.IsValid())
				{
					StatPtr->EnforceSprite.LoadSynchronous();
				}

				DeckCardStatTable.Add(StatPtr);
			}
		}

		DeckCardNum = DeckCardStatTable.Num();
		ensure(DeckCardNum > 0);
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> BlindDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/DT_BlindStat.DT_BlindStat'"));
	if (nullptr != BlindDataTableRef.Object)
	{
		const UDataTable* DataTable = BlindDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		for (const auto& Row : DataTable->GetRowMap())
		{
			const FName RowName = Row.Key;
			FBlindStat* StatPtr = reinterpret_cast<FBlindStat*>(Row.Value);
			if (StatPtr)
			{
				BlindStatTable.Add(StatPtr);
			}
		}

		BlindStatNum = BlindStatTable.Num();
		ensure(BlindStatNum > 0);
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> JockerStatTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/DT_JokerStatTable.DT_JokerStatTable'"));
	if (nullptr != JockerStatTableRef.Object)
	{
		const UDataTable* DataTable = JockerStatTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		for (const auto& Row : DataTable->GetRowMap())
		{
			// 나중에 리소스 확보하고 주석해제하기
			const FName RowName = Row.Key;
			FJokerStat* StatPtr = reinterpret_cast<FJokerStat*>(Row.Value);
			if (StatPtr)
			{
				int32 ItemIndex = static_cast<int>(StatPtr->JokerType);
				FString ItemIndexStr = FString::FromInt(ItemIndex);
				FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Joker/Joker%s.Joker%s"), *ItemIndexStr, *ItemIndexStr);
				StatPtr->CardSprite = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
				if (!StatPtr->CardSprite.IsValid())
				{
					StatPtr->CardSprite.LoadSynchronous();
				}


				JokerStatTable.Add(StatPtr);
			}
		}

		JokerStatMaxNum = JokerStatTable.Num();
		ensure(JokerStatMaxNum > 0);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>TaroTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/DT_TaroStat.DT_TaroStat'"));
	if (nullptr != TaroTableRef.Object)
	{
		const UDataTable* DataTable = TaroTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		for (const auto& Row : DataTable->GetRowMap())
		{
			const FName RowName = Row.Key;
			FTaroStat* StatPtr = reinterpret_cast<FTaroStat*>(Row.Value);
			if (StatPtr)
			{
				StatPtr->index = FCString::Atoi(*RowName.ToString());
				FString AssetPath = FString::Printf(TEXT("/Game/CardResuorce/Taro/Tarots_Sprite_%s.Tarots_Sprite_%s"), *RowName.ToString(), *RowName.ToString());
				StatPtr->SpriteAsset = TSoftObjectPtr<UPaperSprite>(FSoftObjectPath(*AssetPath));
				TaroStatTable.Add(StatPtr);
			}
		}

		TaroStatMaxNum = TaroStatTable.Num();
		ensure(TaroStatMaxNum > 0);
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
