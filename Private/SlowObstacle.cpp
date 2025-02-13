// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowObstacle.h"
#include "Kismet/GameplayStatics.h"

void ASlowObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void ASlowObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlowObstacle::ActivateObstacle(AActor* Activator)
{
	Super::ActivateObstacle(Activator);

	if (Activator && Activator->ActorHasTag("Player")) {
		Activator->CustomTimeDilation = 0.3f;
	}
}

void ASlowObstacle::DeactivateObstacle(AActor* Activator)
{
	Super::DeactivateObstacle(Activator);

	if (Activator && Activator->ActorHasTag("Player")) {
		Activator->CustomTimeDilation = 1.0f;
	}
}
