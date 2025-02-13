// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObstacleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObstacleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLEACTOR_API IObstacleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnMeshOverlap(
		UPrimitiveComponent* OverlapComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBoydIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnMeshEndOverlap(
		UPrimitiveComponent* OverlapComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	virtual void ActivateObstacle(AActor* Activator) = 0;
	virtual void DeactivateObstacle(AActor* Activator) = 0;
};
