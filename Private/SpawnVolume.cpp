// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Scene);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	// 데이터 테이블 모든 행 가져오기
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	// 전체 확률 확인하기
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows) {
		if (Row) {
			TotalChance += Row->SpawnChance;
		}
	}

	// 랜덤값 받아오기
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	
	// 누적 확률 아이템 선택
	for (FItemSpawnRow* Row : AllRows) {
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
			return Row;
	}

	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawnBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnBox->GetComponentLocation();
	FVector SpawnLocation = BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);

	return SpawnLocation;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectRow = GetRandomItem()) {
		if (UClass* ActualClass = SelectRow->ItemClass.Get()) {
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),	// 랜덤 소환 위치
		FRotator::ZeroRotator		// 회전값 0
	);

	return SpawnedItem;
}

void ASpawnVolume::SpawnCoinItem()
{
	if (!CoinClass) return;

	AActor* SpawnedCoin = GetWorld()->SpawnActor<AActor>(
		CoinClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
}

void ASpawnVolume::SpawnHealItem()
{
	if(!HealClass) return;

	AActor* SpawnHeal = GetWorld()->SpawnActor<AActor>(
		HealClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
}
