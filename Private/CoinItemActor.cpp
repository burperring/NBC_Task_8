// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItemActor.h"
#include "Engine/World.h"
#include "SpartaGameState.h"

ACoinItemActor::ACoinItemActor()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItemActor::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player")) {
		if (UWorld* World = GetWorld()) {
			ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>();
			if (GameState) {
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		
		DestroyItem();
	}
}
