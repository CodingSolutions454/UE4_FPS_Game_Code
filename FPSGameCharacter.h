// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/inputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AFPSGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USceneComponent* hand_r;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	// This is So that camera can follow my player.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* AimCamera;

public:
	AFPSGameCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSGameProjectile_1> ProjectileClass;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AGrenade_1> GrenadeClass;
	
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector Weapon_1_SpawnOffset;
	
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	////////////////////////// floats
	
	//Character health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CharacterHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CharacterHealth_Treshold;
	
	//Primary amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PrimaryAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PrimaryAmmo_Treshold;
	
	//Grenade amount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Grenades;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Grenades_Treshold;


protected:
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	// Set up for this actors basic collisions
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);


	// Bool Conditions 
	bool Aiming;
	bool FiringAnimation;
	bool bDead;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	///////////////////////////////////////////////////// Animations
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* FirePrimaryAnimation;

	/** AnimMontage to play each time we reload */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* ReloadPrimaryAnimation;

	/** AnimMontage to play each time we Aim */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* AimAnimation;
	
	/** AnimMontage to play each time we Zoom out */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* ZoomOutPrimaryAnimation;

	/** AnimMontage to play each time we Zoom out */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* ThrowAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_1> Weapon1class;
	class AWeapon_1* Weapon_1;

	// Can restart game after my character death.
	void RestartGame();

protected:
	
	// Input

	/** Fires a projectile. */
	void OnFire();

	/** Throws a grenade. */
	void OnThrowGrenade();

	//** Reloads primary weapon
	void OnReloadPrimary();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	
	// Set aim camera active
	void AimCamActive();
	
	// Set first person camera active
	void FirstPersonCamActive();
};

