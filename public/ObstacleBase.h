// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleInterface.h"
#include "ObstacleBase.generated.h"

UCLASS()
class PUZZLEACTOR_API AObstacleBase : public AActor, public IObstacleInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float RazerDamage;

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
	virtual void ActivateObstacle(AActor* Activator) override;
	virtual void DeactivateObstacle(AActor* Activator) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
