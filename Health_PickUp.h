// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Health_PickUp.generated.h"

UCLASS()
class FPSGAME_API AHealth_PickUp : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHealth_PickUp();

	// declare our float variables 	
	UPROPERTY(EditAnywhere, Category = Movement)
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
		float RollValue;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component for Missile Class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* PickUpMesh;

};
