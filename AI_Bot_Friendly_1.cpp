// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Friendly_1.h"
#include "AI_Bot_Enemy_1.h"
#include "Animation/AnimInstance.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "FPSGameProjectile_1.h"
#include "Components/BoxComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon_1.h"
#include "WayPoint.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AAI_Bot_Friendly_1::AAI_Bot_Friendly_1()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{

	}

	// Set capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 82.0f);

	// Set platers rotations
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character Movement values
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	// Create the head collsion box
	HeadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Head"));
	HeadCollision->SetupAttachment(GetRootComponent());
	HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Friendly_1::OnHeadCollision);

	bDead = false;
	IsFiring = false;
	Health = 100.0f;

	// This characters walk speed
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// Default offset from the character location for projectiles to spawn
	Weapon_1_SpawnOffset = FVector(100.0f, 15.0f, 40.0f);

	//Create Pawn sensing component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensing->SensingInterval = .5f; //  times per second
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->SetPeripheralVisionAngle(85.f);
}

// This actors basic collisions
void AAI_Bot_Friendly_1::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	// if we hit this projectile then remove 40 health
	if (OtherActor->ActorHasTag("Projectile_2"))
	{
		Health -= 40.0f;

		if (Health > 100.0f)
			Health = 100.0f;
	}
}


// This actors head collision
void AAI_Bot_Friendly_1::OnHeadCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	
	// If we this character head hits this projectile then remove all health
	if (OtherActor->ActorHasTag("Projectile_2"))
	{
		Health -= 101.0f;

		if (Health > 100.0f)
			Health = 100.0f;
	}
}

// Called when the game starts or when spawned
void AAI_Bot_Friendly_1::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Friendly_1::SeePawn);

	// Get this actors capsule
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Friendly_1::OnBeginOverLap);


	// Attach our items to our character on spawn
	FActorSpawnParameters SpawnParems;
	SpawnParems.bNoFail = true;
	SpawnParems.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Attach PrimaryWeaponclass to the chosen bone socket
	if (Weapon1class)
	{
		FTransform Primary_WeaponTransform;

		Primary_WeaponTransform.SetLocation(FVector::ZeroVector);
		Primary_WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Weapon_1 = GetWorld()->SpawnActor<AWeapon_1>(Weapon1class, Primary_WeaponTransform, SpawnParems);
		if (Weapon_1)
		{
			Weapon_1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_Weapon_1"));
		}
	}
}

// Pawn sensing component logic
void AAI_Bot_Friendly_1::SeePawn(APawn* Pawn)
{
	if (PawnSensing->OnSeePawn, true) 
	{
		AAI_Bot_Enemy_1* Actor = Cast<AAI_Bot_Enemy_1>(PawnSensing);
		
		IsFiring = true;

		// try and fire a projectile
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{

				{

					const FRotator SpawnRotation = GetControlRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = ((hand_r != nullptr) ? hand_r->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(Weapon_1_SpawnOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the projectile at the muzzle
					World->SpawnActor<AFPSGameProjectile_1>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
	
	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Play Firing Animation
	GetMesh()->SetAnimation(FirePrimaryAnimation);
	GetMesh()->GetAnimInstance()->Montage_Play(FirePrimaryAnimation);
}

// Called every frame
void AAI_Bot_Friendly_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If our health is 0. Then simulate phisics
	if (Health < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			Controller->Destroy();

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

}

// Set this actors view point
void AAI_Bot_Friendly_1::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");

	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}
