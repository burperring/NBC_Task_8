// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "SlowObstacle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEACTOR_API ASlowObstacle : public AObstacleBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void ActivateObstacle(AActor* Activator) override;
	virtual void DeactivateObstacle(AActor* Activator) override;
};
