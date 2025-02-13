// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "Components/TimelineComponent.h"
#include "MovementObstacle.generated.h"

/**
 * 
 */
class UCurveFloat;

UCLASS()
class PUZZLEACTOR_API AMovementObstacle : public AObstacleBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void TimelineProgress(float Value);

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void MoveEvent();
protected:

	FTimeline MoveTimeline;
	UPROPERTY(EditAnywhere, Category = "Obstacle|Timeline")
	UCurveFloat* MoveCurve;

	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector EndLocation;

public:
	UPROPERTY(EditAnywhere, Category = "Obstacle|Timeline")
	float fOffset_X;
	UPROPERTY(EditAnywhere, Category = "Obstacle|Timeline")
	float fOffset_Y;
	UPROPERTY(EditAnywhere, Category = "Obstacle|Timeline")
	float fOffset_Z;
	UPROPERTY(EditAnywhere, Category = "Obstacle|Timeline")
	float ObstacleSpeed = 1;

protected:
	virtual void ActivateObstacle(AActor* Activator) override;
};
