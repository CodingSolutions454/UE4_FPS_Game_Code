

#include "FPSGameCharacter.h"
#include "Weapon_1.h"
#include "Animation/AnimInstance.h"
#include "FPSGameProjectile_1.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Grenade_1.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFPSGameCharacter

AFPSGameCharacter::AFPSGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Character movement values
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create aim camera component
	AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCamera"));
	AimCamera->SetupAttachment(AimCamera, USpringArmComponent::SocketName);
	AimCamera->SetupAttachment(GetMesh(), FName("hand_r_Weapon_1"));
	AimCamera->bUsePawnControlRotation = true;

	// bool conditions
	Aiming = false;
	FiringAnimation = false;
	bDead = false;

	// Float values
	CharacterHealth = 100.0f;
	PrimaryAmmo = 13.0f;
	Grenades = 4.0f;

	// Default offset from the character location for projectiles to spawn
	Weapon_1_SpawnOffset = FVector(200.0f, 10.0f, -10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void AFPSGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Our capsule component for are BeginOnOverlap event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFPSGameCharacter::OnBeginOverLap);
	
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

void AFPSGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If our health is 0. Then simulate phisics and after seconds restart the game
	if (CharacterHealth < 0) {

		if (!bDead) {

			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AFPSGameCharacter:: RestartGame, 1.0f, false);
		}
	}

	// Reload this weapon once we have less then 1 bullet and play the relaod animation
	if (PrimaryAmmo < 1) 
	{
		GetMesh()->SetAnimation(ReloadPrimaryAnimation);
		GetMesh()->GetAnimInstance()->Montage_Play(ReloadPrimaryAnimation);

		// If we relaod then increase the weapons ammmo amount for this weapon
		PrimaryAmmo += 13.0f;
	}
}

// Basic collisions for this actor
void AFPSGameCharacter::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult)
{
	// if we hit this projectile then remove 15 health
	if (OtherActor->ActorHasTag("Enemy_Projectile"))
	{
		CharacterHealth -= 5.0f;

		if (CharacterHealth > 100.0f)
			CharacterHealth = 100.0f;
	}

	// if we hit this projectile then remove 15 health
	if (OtherActor->ActorHasTag("HealthPickUp"))
	{
		CharacterHealth += 100.0f;

		if (CharacterHealth > 100.0f)
			CharacterHealth = 100.0f;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPSGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("AimCamActive", IE_Pressed, this, &AFPSGameCharacter::AimCamActive);
	
	PlayerInputComponent->BindAction("FirstPersonCamActive", IE_Released, this, &AFPSGameCharacter::FirstPersonCamActive);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSGameCharacter::OnFire);
	PlayerInputComponent->BindAction("ReloadPrimary", IE_Pressed, this, &AFPSGameCharacter::OnReloadPrimary);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AFPSGameCharacter::OnThrowGrenade);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFPSGameCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSGameCharacter::LookUpAtRate);
}

// Aim camera set active
void AFPSGameCharacter::AimCamActive()
{
	Aiming = true;
	
	AimCamera->SetActive(true);
	FirstPersonCameraComponent->SetActive(false);

	GetMesh()->SetAnimation(AimAnimation);
	GetMesh()->SetPlayRate(1.f);
	GetMesh()->GetAnimInstance()->Montage_Play(AimAnimation);
}

// First person camera set active
void AFPSGameCharacter::FirstPersonCamActive()
{
	Aiming = false;

	FirstPersonCameraComponent->SetActive(true);
	AimCamera->SetActive(false);

	GetMesh()->SetAnimation(ZoomOutPrimaryAnimation);
	GetMesh()->SetPlayRate(1.f);
	GetMesh()->GetAnimInstance()->Montage_Play(ZoomOutPrimaryAnimation);
}

void AFPSGameCharacter::RestartGame()
{
	// If health is 0. Restart game and open world
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

}

// Fire primary Weapon functionality
void AFPSGameCharacter::OnFire()
{
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

	// Lose one ammo for everytime we fire the primary weapon
	PrimaryAmmo -= 1.0f;

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// if we are aiming then stop the sway of the character animation
	if (Aiming == false) 
	{
		GetMesh()->SetAnimation(FirePrimaryAnimation);
		GetMesh()->GetAnimInstance()->Montage_Play(FirePrimaryAnimation); 
	}
}

void AFPSGameCharacter::OnThrowGrenade()
{
	// try and fire a projectile
	if (GrenadeClass != nullptr)
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
				World->SpawnActor<AGrenade_1>(GrenadeClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// Remove 1 grande for each time we throw a grenade
	Grenades -= 1.0f;

	// if we are aiming then stop the sway of the character animation
	if (Aiming == false)
	{
		GetMesh()->SetAnimation(ThrowAnimation);
		GetMesh()->GetAnimInstance()->Montage_Play(ThrowAnimation);
	}

}

// Reload primary weapon functionality. Play reload animation and relaod the guns clip for the primary weapon
void AFPSGameCharacter::OnReloadPrimary()
{
	FirstPersonCameraComponent->SetActive(true);
	AimCamera->SetActive(false);

	GetMesh()->SetAnimation(ZoomOutPrimaryAnimation);
	GetMesh()->SetPlayRate(1.f);
	GetMesh()->GetAnimInstance()->Montage_Play(ZoomOutPrimaryAnimation);
	
	GetMesh()->SetAnimation(ReloadPrimaryAnimation);
	GetMesh()->GetAnimInstance()->Montage_Play(ReloadPrimaryAnimation);

	PrimaryAmmo = 13.0f;
}

// Restart the game and open level
void AFPSGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AFPSGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

