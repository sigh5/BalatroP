// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyPawn.h"
#include "Character/Component/TakeDamageComponent.h"
#include "Character/Component/HandRankingComponent.h"
#include "Character/Component/PlayerInfoComponent.h"
#include "Character/Component/CardAndDeckComponent.h"
#include "Character/Component/CalculatorHandRankingComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
	const auto DamageComponent = CreateDefaultSubobject<UTakeDamageComponent>("TakeDamage");
	const auto HandRankingComponent = CreateDefaultSubobject<UHandRankingComponent>("HandRanking");
	const auto PlayerInfoComponent = CreateDefaultSubobject<UPlayerInfoComponent>("PlayerInfo");
	const auto CardAndDeckComponent = CreateDefaultSubobject<UCardAndDeckComponent>("CardDeck");
	const auto CalculatorHandPlayComponent = CreateDefaultSubobject<UCalculatorHandRankingComponent>("CalculatorHandPlay");
}
