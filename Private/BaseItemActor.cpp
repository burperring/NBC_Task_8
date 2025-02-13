#include "BaseItemActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItemActor::ABaseItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	// Default 콜리전 프리셋 설정
	Collision->SetupAttachment(Scene);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);

	// 이벤트 바인딩(
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItemActor::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItemActor::OnItemEndOverlap);
}

void ABaseItemActor::OnItemOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player")) {
		ActivateItem(OtherActor);
	}
}

void ABaseItemActor::OnItemEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseItemActor::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle) {
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}

	if (PickupSound) {
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}

	if (Particle) {
		FTimerHandle DestroyParticleHandle;
		
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleHandle,
			[Particle]() {
				Particle->DestroyComponent();
			},
			10.0f,
			false
		);
	}
}

FName ABaseItemActor::GetItemType() const
{
	return ItemType;
}

void ABaseItemActor::DestroyItem()
{
	Destroy();
}
