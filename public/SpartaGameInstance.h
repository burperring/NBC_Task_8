// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEACTOR_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USpartaGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION()
	void AddToScore(int32 Amount);
};
