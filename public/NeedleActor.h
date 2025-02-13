// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleInterface.h"
#include "Components/TimelineComponent.h"
#include "NeedleActor.generated.h"

class UBoxComponent;

UCLASS()
class PUZZLEACTOR_API ANeedleActor : public AActor, public IObstacleInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeedleActor();

	UFUNCTION()
	void TimelineProgress(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Needle")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Needle")
	UStaticMeshComponent* FloorMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Needle")
	UBoxComponent* BoxCollision;

	FTimeline MoveTimeline;
	UPROPERTY(EditAnywhere, Category = "Needle|Timeline")
	UCurveFloat* MoveCurve;
	UPROPERTY(EditAnywhere, Category = "Needle|Timeline")
	float Offset_Z;
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector EndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needle|Effects")
	USoundBase* DelaySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needle")
	UMaterialInterface* WarningMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needle")
	UMaterialInterface* NomalMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needle")
	float NeedleDamage;

	virtual void OnMeshOverlap(
		UPrimitiveComponent* OverlapComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBoydIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnMeshEndOverlap(
		UPrimitiveComponent* OverlapComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

public:	
	virtual void ActivateObstacle(AActor* Activator) override;
	virtual void DeactivateObstacle(AActor* Activator) override;
	void MoveEvent();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void NeedleEventStart();

};
