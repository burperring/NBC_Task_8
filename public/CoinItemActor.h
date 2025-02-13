// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemActor.h"
#include "CoinItemActor.generated.h"

UCLASS()
class PUZZLEACTOR_API ACoinItemActor : public ABaseItemActor
{
	GENERATED_BODY()
	
public:
	ACoinItemActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};
