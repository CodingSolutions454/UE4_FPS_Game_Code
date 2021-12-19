// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Enemy_Controller_1.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AAI_Enemy_Controller_1 : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Enemy_Controller_1();

	// Override this function 
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* OnPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;
	
	// OnPawn detected condition
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawn);

	//AI Sights Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AILosesRadius = AISightAge + 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfview = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;
};
