// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Enemy_1.h"
#include "AI_Bot_Friendly_1.h"
#include "Head_1.h"
#include "LowerLeftArm_1.h"
#include "LowerLeftLeg_1.h"
#include "LowerRightArm_1.h"
#include "LowerRightLeg_1.h"
#include "UpperTorso_1.h"
#include "LowerTorso_1.h"
#include "UpperLeftArm_1.h"
#include "UpperLeftLeg_1.h"
#include "UpperRightArm_1.h"
#include "UpperRightLeg_1.h"
#include "Animation/AnimInstance.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "FPSGameCharacter.h"
#include "FPSGameProjectile_2.h"
#include "Components/BoxComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon_1.h"
#include "WayPoint.h"


// Sets default values
AAI_Bot_Enemy_1::AAI_Bot_Enemy_1(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy_1::OnHeadCollision);

	bDead = false;
	IsFiring = false;
	EnemyHealth = 100.0f;
	
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
void AAI_Bot_Enemy_1::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	
	// If we hit a grenade, then remove all health
	if (OtherActor->ActorHasTag("Grenade_1"))
	{
		// Remove all of this characters health
		EnemyHealth -= 101.0f;

		if (EnemyHealth > 100.0f)
			EnemyHealth = 100.0f;
		
		// Destoy this actor and its weapons
		Destroy();
		Weapon_1->Destroy();

		// if This character has a collision with the chosen enemy. Then create this particle effect and destroy it and its capsule
		UParticleSystemComponent* Blood = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSystem, GetActorTransform());

		// try and fire a projectile
		if (BodyPartClass1 != nullptr)
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

					// spawn these body parts when this actor collides with a grenade
					World->SpawnActor<AHead_1>(BodyPartClass1, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<ALowerLeftArm_1>(BodyPartClass2, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<ALowerLeftLeg_1>(BodyPartClass3, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<ALowerRightArm_1>(BodyPartClass4, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<ALowerRightLeg_1>(BodyPartClass5, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<AUpperTorso_1>(BodyPartClass6, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<ALowerTorso_1>(BodyPartClass7, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<AUpperLeftArm_1>(BodyPartClass8, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<AUpperLeftLeg_1>(BodyPartClass9, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<AUpperRightArm_1>(BodyPartClass10, SpawnLocation, SpawnRotation, ActorSpawnParams);
					World->SpawnActor<AUpperRightLeg_1>(BodyPartClass11, SpawnLocation, SpawnRotation, ActorSpawnParams);
					
				}
			}
		}
	
	}
	
	// if we hit this projectile then remove 40 health
	if (OtherActor->ActorHasTag("Projectile_1"))
	{
		EnemyHealth -= 40.0f;

		if (EnemyHealth > 100.0f)
			EnemyHealth = 100.0f;
	}
}


// This actors head collision
void AAI_Bot_Enemy_1::OnHeadCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	
	// If we this character head hits this projectile then remove all health
	if (OtherActor->ActorHasTag("Projectile_1"))
	{
		EnemyHealth -= 101.0f;

		if (EnemyHealth > 100.0f)
			EnemyHealth = 100.0f;
	}
}

UParticleSystemComponent* AAI_Bot_Enemy_1::CreateBlood(UParticleSystem* Blood)
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
void AAI_Bot_Enemy_1::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &AAI_Bot_Enemy_1::SeePawn);

	// Get this actors capsule
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAI_Bot_Enemy_1::OnBeginOverLap);

	
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

// Pawn sensing component used for the sword attack
void AAI_Bot_Enemy_1::SeePawn(APawn* Pawn)
{
	if (PawnSensing->OnSeePawn, true)
	{
		AAI_Bot_Friendly_1* Actor = Cast<AAI_Bot_Friendly_1>(PawnSensing); 
		
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
					World->SpawnActor<AFPSGameProjectile_2>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
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

	if (PawnSensing->OnSeePawn, true)
	{
		AFPSGameCharacter* Actor = Cast<AFPSGameCharacter>(PawnSensing);

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
					World->SpawnActor<AFPSGameProjectile_2>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
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
void AAI_Bot_Enemy_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If our health is 0. Then simulate phisics
	if (EnemyHealth < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			Controller->Destroy();

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Weapon_1->Destroy();
		}
	}
	
}

// Set this actors view point
void AAI_Bot_Enemy_1::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation("head");

	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform("head", RTS_ParentBoneSpace).Rotator().Roll;
}
