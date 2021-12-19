// Fill out your copyright notice in the Description page of Project Settings.


#include "OnHitBlood_FX.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOnHitBlood_FX::AOnHitBlood_FX()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);
}

// Collision logic for this actor
void AOnHitBlood_FX::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("AI_Bot_Enemy_1"))
	{
		// if This character has a collision with the chosen enemy. Then create this particle effect and destroy it and its capsule
		UParticleSystemComponent* Blood = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSystem, GetActorTransform());

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (OtherActor->ActorHasTag("Our_Character"))
	{
		// if This character has a collision with the chosen enemy. Then create this particle effect and destroy it and its capsule
		UParticleSystemComponent* Blood = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSystem, GetActorTransform());

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Create blood system particle system
UParticleSystemComponent* AOnHitBlood_FX::CreateBlood(UParticleSystem* Blood)
{
	class UParticleSystemComponent* retVal = NULL;

	if (BloodSystem)
	{
		class UWorld* const world = GetWorld();

		if (world)
		{
			FVector myPos = GetActorLocation();
			FRotator myRot = GetActorRotation();

			retVal = UGameplayStatics::SpawnEmitterAtLocation(world, BloodSystem, myPos, myRot, true);
		}
	}

	return retVal;
}

// Called when the game starts or when spawned
void AOnHitBlood_FX::BeginPlay()
{
	Super::BeginPlay();

	// Our capsule component for are BeginOnOverlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOnHitBlood_FX::OnBeginOverLap);
}

// Called every frame
void AOnHitBlood_FX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
