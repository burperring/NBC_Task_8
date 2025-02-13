// Fill out your copyright notice in the Description page of Project Settings.


#include "HealItemActor.h"
#include "SpartaCharacter.h"

AHealItemActor::AHealItemActor()
{
	HealAmount = 20.0f;
	ItemType = "HealItem";
}

void AHealItemActor::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player")) {
		ASpartaCharacter* Player = Cast<ASpartaCharacter>(Activator);

		if (Player) {
			Player->AddHealth(HealAmount);
		}

		DestroyItem();
	}
}
