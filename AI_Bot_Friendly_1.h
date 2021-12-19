// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/inputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/AISightTargetInterface.h"
#include "AI_Bot_Friendly_1.generated.h"

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AAI_Bot_Friendly_1 : public ACharacter
{
	GENERATED_BODY()

		/** Location on gun mesh where projectiles should spawn. */
		UPROPERTY(EditDefaultsOnly, Category = Mesh)
		USceneComponent* hand_r;

public:
	// Sets default values for this character's properties
	AAI_Bot_Friendly_1();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSGameProjectile_1> ProjectileClass;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector Weapon_1_SpawnOffset;

	// Waypoint for actor using this waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWayPoint* NextWayPoint;
	
	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	// Bools
	bool bDead;
	bool  bDoOnce;
	bool IsFiring;

	UPROPERTY(EditAnywhere)
		float Health;

	//Weapon 1 class
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_1> Weapon1class;
	class AWeapon_1* Weapon_1;

	// Set up for this actors basic collisions
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// Head collisions
	UFUNCTION()
		void OnHeadCollision(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);


	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimMontage* FirePrimaryAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Create Pawnsensing component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	// For OnPawn Detected
	UFUNCTION()
		void SeePawn(APawn* Pawn);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Head collision box
	UPROPERTY(EditAnywhere)
		class UBoxComponent* HeadCollision;

	// AI seeing is attached this players head
	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;
};
