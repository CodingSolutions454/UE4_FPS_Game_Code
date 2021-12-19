// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LowerRightArm_1.generated.h"

UCLASS()
class FPSGAME_API ALowerRightArm_1 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALowerRightArm_1();

	// Actor body part
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* BodyPart;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
