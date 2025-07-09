// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyPawn.h"
#include "Character/TakeDamageComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
	const auto DamageComponent = CreateDefaultSubobject<UTakeDamageComponent>("TakeDamage");
}