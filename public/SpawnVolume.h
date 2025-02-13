// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "ObsacleArrayStruct.h"
#include "NeedleActor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class PUZZLEACTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	UBoxComponent* SpawnBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TSubclassOf<AActor> CoinClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
	TSubclassOf<AActor> HealClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnVolume|Wave")
	TArray<FObsacleArrayStruct> WaveSpawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnVolume|Needle")
	TArray<AActor*> NeedleActor;

	FItemSpawnRow* GetRandomItem() const;
	FVector GetRandomPointInVolume() const;
	UFUNCTION(BlueprintCallable)
	AActor* SpawnRandomItem();
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

	void SpawnCoinItem();
	void SpawnHealItem();
};
