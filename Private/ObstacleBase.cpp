// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObstacleBase::AObstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RazerDamage = 20.0f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AObstacleBase::OnMeshOverlap);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &AObstacleBase::OnMeshEndOverlap);
}

void AObstacleBase::OnMeshOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBoydIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) {
		ActivateObstacle(OtherActor);
	}
}

void AObstacleBase::OnMeshEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) {
		DeactivateObstacle(OtherActor);
	}
}

void AObstacleBase::ActivateObstacle(AActor* Activator)
{
}

void AObstacleBase::DeactivateObstacle(AActor* Activator)
{
}

// Called every frame
void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

