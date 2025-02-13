// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItemActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItemActor::AMineItemActor()
{
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 40.0f;
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItemActor::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	// 게임 월드 -> 타이머 매니저
	// 타이머 핸들러
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle, this, &AMineItemActor::Explode, ExplosionDelay, false);

}

void AMineItemActor::Explode()
{
	TArray<AActor*> OverlapActors;
	ExplosionCollision->GetOverlappingActors(OverlapActors);

	for (AActor* Actor : OverlapActors) {
		if (Actor && Actor->ActorHasTag("Player")) {
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}

	DestroyItem();
}
