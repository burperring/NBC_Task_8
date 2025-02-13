// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItemActor.h"
#include "BigCoinItemActor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEACTOR_API ABigCoinItemActor : public ACoinItemActor
{
	GENERATED_BODY()
	
public:
	ABigCoinItemActor();

	virtual void ActivateItem(AActor* Activator) override;
};
