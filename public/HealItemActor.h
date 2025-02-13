// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemActor.h"
#include "HealItemActor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEACTOR_API AHealItemActor : public ABaseItemActor
{
	GENERATED_BODY()
	
public:
	AHealItemActor();

	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;
};
