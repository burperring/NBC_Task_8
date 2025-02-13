// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateObstacle.h"
#include "Kismet/GameplayStatics.h"

void ARotateObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void ARotateObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);

	FQuat QuatRotation = FQuat(NewRotation);

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void ARotateObstacle::ActivateObstacle(AActor* Activator)
{
	Super::ActivateObstacle(Activator);

	if (Activator && Activator->ActorHasTag("Player")) {
		UGameplayStatics::ApplyDamage(
			Activator,
			RazerDamage,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}
}
