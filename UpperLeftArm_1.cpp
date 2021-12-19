// Fill out your copyright notice in the Description page of Project Settings.


#include "UpperLeftArm_1.h"

// Sets default values
AUpperLeftArm_1::AUpperLeftArm_1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Body part mesh
	BodyPart = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = BodyPart;

	// Die after 40 seconds by default
	InitialLifeSpan = 40.0f;
}

// Called when the game starts or when spawned
void AUpperLeftArm_1::BeginPlay()
{
	Super::BeginPlay();

	// Stimulate physics of this body part
	BodyPart->SetSimulatePhysics(true);
}

// Called every frame
void AUpperLeftArm_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}