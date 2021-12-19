// Fill out your copyright notice in the Description page of Project Settings.


#include "Health_PickUp.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHealth_PickUp::AHealth_PickUp()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this rotation
	PitchValue = 0.f;
	YawValue = -2.f;
	RollValue = 0.f;

	// Creates a body for this mesh and is attacted to the root component
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup"));
	PickUpMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealth_PickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealth_PickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// on every frame change rotationg for a smooth rotating actor
	FQuat QuatRotation = FRotator(PitchValue, YawValue, RollValue).Quaternion();

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

}

