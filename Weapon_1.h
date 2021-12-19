// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon_1.generated.h"

UCLASS()
class FPSGAME_API AWeapon_1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWeapon_1();

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		USceneComponent* Muzzle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
