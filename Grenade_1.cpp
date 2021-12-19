// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade_1.h"
#include "Explosion_1.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrenade_1::AGrenade_1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a body for this mesh and is attacted to the root component
	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Projectile->SetupAttachment(RootComponent);

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade_1::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = true;

	// Die after 10 seconds by default
	InitialLifeSpan = 10.0f;
}

// Create this particle effect on impact
UParticleSystemComponent* AGrenade_1::CreateExplosion(UParticleSystem* Explosion)
{

	class UParticleSystemComponent* retVal = NULL;

	if (ExplosionSystem)
	{
		class UWorld* const world = GetWorld();

		if (world)
		{
			FVector myPos = GetActorLocation();
			FRotator myRot = GetActorRotation();

			retVal = UGameplayStatics::SpawnEmitterAtLocation(world, ExplosionSystem, myPos, myRot, true);
		}
	}

	return retVal;
}

void AGrenade_1::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
	
	// if This character has a collision with the chosen enemy. Then create this particle effect and destroy it and its capsule
	UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionSystem, GetActorTransform());

	// try and play the sound if specified
	if (ExplosionSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}

void AGrenade_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

