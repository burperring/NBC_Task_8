// Fill out your copyright notice in the Description page of Project Settings.


#include "NeedleActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANeedleActor::ANeedleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NeedleDamage = 20.0f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ANeedleActor::OnMeshOverlap);
}

void ANeedleActor::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Value);
	BoxCollision->SetRelativeLocation(NewLocation);
}

// Called when the game starts or when spawned
void ANeedleActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANeedleActor::OnMeshOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBoydIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) {
		UGameplayStatics::ApplyDamage(
			OtherActor,
			NeedleDamage,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}
}

void ANeedleActor::OnMeshEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ANeedleActor::ActivateObstacle(AActor* Activator)
{
	if (DelaySound) {
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			DelaySound,
			GetActorLocation()
		);
	}

	FloorMesh->SetMaterial(0, WarningMaterial);

	FTimerHandle MoveNeddleHandle;
	GetWorld()->GetTimerManager().SetTimer(
		MoveNeddleHandle, this, &ANeedleActor::MoveEvent, 3.0f, false
	);
}

void ANeedleActor::DeactivateObstacle(AActor* Activator)
{
}

void ANeedleActor::MoveEvent()
{
	FloorMesh->SetMaterial(0, NomalMaterial);

	if (MoveCurve) {
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		MoveTimeline.AddInterpFloat(MoveCurve, TimelineProgress);
		MoveTimeline.SetLooping(false);

		StartLocation = EndLocation = BoxCollision->GetRelativeLocation();
		EndLocation.Z += Offset_Z;

		MoveTimeline.PlayFromStart();
		MoveTimeline.SetPlayRate(1.0f);
	}
}

// Called every frame
void ANeedleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveTimeline.TickTimeline(DeltaTime);
}

void ANeedleActor::NeedleEventStart()
{
	ActivateObstacle(this);
}
