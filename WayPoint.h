// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WayPoint.generated.h"

UCLASS()
class FPSGAME_API AWayPoint : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWayPoint();

	// Scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;

	// Box component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	// Waypoint for actor using this waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		 AWayPoint* NextWayPoint;

protected:
	
	// Collision event for when player enters
	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
