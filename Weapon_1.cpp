// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_1.h"
#include "FPSGameProjectile_1.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon_1::AWeapon_1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon_1::BeginPlay()
{
	Super::BeginPlay();
}

