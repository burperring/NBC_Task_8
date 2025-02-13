// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementObstacle.h"
#include "Kismet/GameplayStatics.h"

void AMovementObstacle::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	SetActorLocation(NewLocation);
}

void AMovementObstacle::BeginPlay()
{
	Super::BeginPlay();

	MoveEvent();
}

void AMovementObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveTimeline.TickTimeline(DeltaTime);
}

void AMovementObstacle::MoveEvent()
{
	if (MoveCurve) {
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		MoveTimeline.AddInterpFloat(MoveCurve, TimelineProgress);
		MoveTimeline.SetLooping(true);

		StartLocation = EndLocation = GetActorLocation();
		EndLocation.X += fOffset_X;
		EndLocation.Y += fOffset_Y;
		EndLocation.Z += fOffset_Z;

		MoveTimeline.PlayFromStart();
		MoveTimeline.SetPlayRate(ObstacleSpeed);
	}
}

void AMovementObstacle::ActivateObstacle(AActor* Activator)
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
