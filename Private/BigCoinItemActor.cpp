// Fill out your copyright notice in the Description page of Project Settings.


#include "BigCoinItemActor.h"

ABigCoinItemActor::ABigCoinItemActor()
{
	PointValue = 50;
	ItemType = "BigCoin";
}

void ABigCoinItemActor::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}

