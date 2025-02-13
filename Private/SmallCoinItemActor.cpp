// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallCoinItemActor.h"

ASmallCoinItemActor::ASmallCoinItemActor()
{
	PointValue = 10;
	ItemType = "SmallCoin";
}

void ASmallCoinItemActor::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}
