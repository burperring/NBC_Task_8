// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObsacleSpawnStruct.generated.h"

/**
 * 
 */
class PUZZLEACTOR_API ObsacleSpawnStruct
{
public:
	ObsacleSpawnStruct();
	~ObsacleSpawnStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FObsacleSpawnStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SpawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float X_PitchValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Y_YawValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Z_RollValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
};
