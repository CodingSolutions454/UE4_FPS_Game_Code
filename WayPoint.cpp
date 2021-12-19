// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPoint.h"
#include "AI_Bot_Enemy_1.h"

// Sets default values
AWayPoint::AWayPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// the scene component to the root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	// Attatch the Box Component to the the root component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWayPoint::OnPlayerEnter);
}

// If the actor has reached this waypoint. Then move to the next waypoint
void AWayPoint::OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAI_Bot_Enemy_1* Character = nullptr;

	if (OtherActor != nullptr)
	{
		Character = Cast<AAI_Bot_Enemy_1>(OtherActor);
		if (Character != nullptr)
		{
			Character->NextWayPoint = NextWayPoint;
		}
	}
}

