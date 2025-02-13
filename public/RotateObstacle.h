// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "RotateObstacle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEACTOR_API ARotateObstacle : public AObstacleBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Obstacle|Movement")
	float PitchValue;
	UPROPERTY(EditAnywhere, Category = "Obstacle|Movement")
	float YawValue;
	UPROPERTY(EditAnywhere, Category = "Obstacle|Movement")
	float RollValue;

	virtual void ActivateObstacle(AActor* Activator) override;
};
