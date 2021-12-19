// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Friendly_Controller_1.h"
#include "AI_Bot_Friendly_1.h"
#include "AI_Bot_Enemy_1.h"
#include "AI_Enemy_Controller_1.h"
#include "FPSGameCharacter.h"
#include "Waypoint.h"
#include "GenericTeamAgentInterface.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_Friendly_Controller_1::AAI_Friendly_Controller_1()
{
	PrimaryActorTick.bCanEverTick = true;
	{
		SetGenericTeamId(FGenericTeamId(1));
	}

	// Create the Sights and perception component
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	// Sight Configeration
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILosesRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfview;
	SightConfig->SetMaxAge(AISightAge);

	// Sightdetection conditions
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;/////////////////////////////////////////
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Friendly_Controller_1::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

ETeamAttitude::Type AAI_Friendly_Controller_1::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID == 2) {
				return ETeamAttitude::Neutral;
			}
			else {
				return ETeamAttitude::Hostile;
			}


		}
	}

	return ETeamAttitude::Neutral;
}

void AAI_Friendly_Controller_1::BeginPlay()
{
	Super::BeginPlay();

	// Get perception component or log probelm
	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Occured"));
	}
}

void AAI_Friendly_Controller_1::OnPossess(APawn* OnPawn)
{
	// Can posses a pawn as a controller
	Super::OnPossess(OnPawn);
}

void AAI_Friendly_Controller_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAI_Bot_Friendly_1* Actor = Cast<AAI_Bot_Friendly_1>(GetPawn());

	// Move to next waypoint
	if (Actor->NextWayPoint != nullptr && bIsPlayerDetected == false)
	{
		MoveToActor(Actor->NextWayPoint, 3.0f);

	}

	if (DistanceToPlayer > AISightRadius)
	{
		// Check this pawns detection
		bIsPlayerDetected = false;

	}
	
	// if detection is true. Move to actor
	if (bIsPlayerDetected == true)
	{
	   // Waypoints will be used for this actor tracking other actors
	}
}

FRotator AAI_Friendly_Controller_1::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		// Get this pawns rotaion and get this actors rotation
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAI_Friendly_Controller_1::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		// On pawn detected get distance to the pawn
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}
	bIsPlayerDetected = true;
}




