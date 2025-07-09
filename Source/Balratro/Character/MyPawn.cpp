// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyPawn.h"
#include "Character/Component/TakeDamageComponent.h"
#include "Character/Component/HandRankingComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
	const auto DamageComponent = CreateDefaultSubobject<UTakeDamageComponent>("TakeDamage");
	const auto HandRankingComponent = CreateDefaultSubobject<UHandRankingComponent>("HandRanking");
}
