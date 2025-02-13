// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObsacleSpawnStruct.h"
#include "ObsacleArrayStruct.generated.h"

/**
 * 
 */
class PUZZLEACTOR_API ObsacleArrayStruct
{
public:
	ObsacleArrayStruct();
	~ObsacleArrayStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FObsacleArrayStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObsacleSpawnStruct> TA;
};
