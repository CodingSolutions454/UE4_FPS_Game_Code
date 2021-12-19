// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OnHitBlood_FX.generated.h"

UCLASS()
class FPSGAME_API AOnHitBlood_FX : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOnHitBlood_FX();

	// Set up for this actors basic collisions
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bfromsweep, const FHitResult& SweepResult);

	// Use engine particle system to create a Blood
	UPROPERTY(EditAnyWhere, Category = "FX")
		class UParticleSystem* BloodSystem;
	class UParticleSystemComponent* CreateBlood(class UParticleSystem* Blood);

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};