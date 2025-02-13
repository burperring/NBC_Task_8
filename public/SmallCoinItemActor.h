// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItemActor.h"
#include "SmallCoinItemActor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEACTOR_API ASmallCoinItemActor : public ACoinItemActor
{
	GENERATED_BODY()
	
public:
	ASmallCoinItemActor();

	virtual void ActivateItem(AActor* Activator) override;
};
