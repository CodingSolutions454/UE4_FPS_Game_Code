// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/inputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/AISightTargetInterface.h"
#include "AI_Bot_Enemy_1.generated.h"

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AAI_Bot_Enemy_1 : public ACharacter
{

	GENERATED_BODY()

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USceneComponent* hand_r;

public:
	// Sets default values for this character's properties
	AAI_Bot_Enemy_1(const FObjectInitializer& ObjectInitializer);

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSGameProjectile_2> ProjectileClass;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass1)
		TSubclassOf<class AHead_1> BodyPartClass1;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass2)
		TSubclassOf<class ALowerLeftArm_1> BodyPartClass2;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass3)
		TSubclassOf<class ALowerLeftLeg_1> BodyPartClass3;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass4)
		TSubclassOf<class ALowerRightArm_1> BodyPartClass4;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass5)
		TSubclassOf<class ALowerRightLeg_1> BodyPartClass5;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass6)
		TSubclassOf<class AUpperTorso_1> BodyPartClass6;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass7)
		TSubclassOf<class ALowerTorso_1> BodyPartClass7;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass8)
		TSubclassOf<class AUpperLeftArm_1> BodyPartClass8;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass9)
		TSubclassOf<class AUpperLeftLeg_1> BodyPartClass9;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass10)
		TSubclassOf<class AUpperRightArm_1> BodyPartClass10;

	/** BodyPart class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = BodyPartClass11)
		TSubclassOf<class AUpperRightLeg_1> BodyPartClass11;
	
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector Weapon_1_SpawnOffset;

	// Waypoint for actor using this waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWayPoint* NextWayPoint;

	// Waypoint for actor using this waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAI_Bot_Enemy_1* Enemy;


	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;
	
	// bools
	bool bDead;
	bool  bDoOnce;
	bool IsFiring;
	
	// Enemy health
	UPROPERTY(EditAnywhere)
		float EnemyHealth;

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

	// Use engine particle system to create a Blood
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* BloodSystem;
	class UParticleSystemComponent* CreateBlood(class UParticleSystem* Blood);
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
