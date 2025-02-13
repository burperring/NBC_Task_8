// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "SpawnVolume.h"
#include "CoinItemActor.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "MovementObstacle.h"
#include "RotateObstacle.h"
#include "NeedleActor.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWave = 0;
	MaxWave = 3;
	LevelDuration = 60.0f;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateHandle,
		this,
		&ASpartaGameState::UpdateHud,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance()) {
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance) {
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::OnGameOver()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController) {
		ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
		if (SpartaPlayerController) {
			SpartaPlayerController->ShowMainMenu(true);
			SpartaPlayerController->SetPause(true);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController) {
		ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
		if (SpartaPlayerController) {
			SpartaPlayerController->ShowGameHud();
		}
	}

	// CurrentLevelIndex 넘겨받기
	if (UGameInstance* GameInstance = GetGameInstance()) {
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance) {
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	// 초기화
	SpawnedCoinCount = 5;
	CollectedCoinCount = 0;

	// 레벨에서 SpawnVolume 찾기
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	// 코인 아이템, 힐 아이템 소환
	if (FoundVolumes.Num() > 0) {
		SpawnObstacle(FoundVolumes[0]);
		SpawnStartItem(FoundVolumes[0]);
	}

	// Activate needle event
	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
	if (!SpawnVolume->NeedleActor.IsEmpty()) {
		GetWorldTimerManager().SetTimer(
			NeedleTimerHandle,
			this,
			&ASpartaGameState::StartNeedleEvent,
			8.0f,
			true
		);
		ActiveNeedle(FoundVolumes[0]);
	}

	/* 아이템 소환 로직& 코인 갯수 세기
	const int32 ItemToSpawn = 40;
	for (int32 i = 0; i < ItemToSpawn; i++) {
		if (FoundVolumes.Num() > 0) {
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume) {
				AActor* SpawnActor = SpawnVolume->SpawnRandomItem();
				if (SpawnActor && SpawnActor->IsA(ACoinItemActor::StaticClass())) {
					SpawnedCoinCount++;
				}
			}
		}
	}*/

	// 게임 시간 타이머 조정
	GetWorldTimerManager().SetTimer(
		LevelTimerHadle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void ASpartaGameState::OnLevelTimeUp()
{
	OnGameOver();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount) {
		CollectedCoinCount = 0;
		EndWave();
	}
	else {
		// SpawnVolume 찾기
		TArray<AActor*> FoundVolumes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

		// 코인 재생성하기
		if (FoundVolumes.Num() > 0) {
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume) {
				SpawnVolume->SpawnCoinItem();
			}
		}
	}
}

void ASpartaGameState::SpawnStartItem(AActor* FoundVolumes)
{
	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes);

	// 코인 아이템, 힐 아이템 소환
	if (SpawnVolume) {
		SpawnVolume->SpawnCoinItem();
		SpawnVolume->SpawnHealItem();
	}
}

void ASpartaGameState::SpawnObstacle(AActor* FoundVolumes)
{
	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes);

	if (SpawnVolume) {
		if (SpawnVolume->WaveSpawnActor.IsValidIndex(CurrentWave)) {
			for (auto& Os : SpawnVolume->WaveSpawnActor[CurrentWave].TA) {
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(
					Os.SpawnActor,
					Os.SpawnLocation,
					Os.SpawnRotation
				);

				// check movement obstacle
				AMovementObstacle* MOs = Cast<AMovementObstacle>(SpawnActor);
				if (MOs) {
					MOs->fOffset_X = Os.X_PitchValue;
					MOs->fOffset_Y = Os.Y_YawValue;
					MOs->fOffset_Z = Os.Z_RollValue;
					MOs->ObstacleSpeed = Os.MoveSpeed;
					MOs->MoveEvent();
				}

				// check rotate obstacle
				ARotateObstacle* ROs = Cast<ARotateObstacle>(SpawnActor);
				if (ROs) {
					ROs->PitchValue = Os.X_PitchValue;
					ROs->YawValue = Os.Y_YawValue;
					ROs->RollValue = Os.Z_RollValue;
				}
			}
		}
	}
}

void ASpartaGameState::StartNeedleEvent()
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	ActiveNeedle(FoundVolumes[0]);
}

void ASpartaGameState::ActiveNeedle(AActor* FoundVolumes)
{
	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes);

	const int32 min = 0;
	const int32 max = 4;

	int32 ActiveCase = FMath::RandRange(min, max);

	switch (ActiveCase) {
	case 0:
		for (int32 Index = 0; Index != SpawnVolume->NeedleActor.Num(); ++Index)
		{
			if (Index % 2 == 0) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
		}
		break;
	case 1: 
		for (int32 Index = 0; Index != SpawnVolume->NeedleActor.Num(); ++Index)
		{
			if (Index % 2 == 1) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
		}
		break;
	case 2:
		for (int32 Index = 0; Index != SpawnVolume->NeedleActor.Num(); ++Index)
		{
			if (Index % 7 == 0 || Index % 5 == 0) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
		}
		break;
	case 3:
		for (int32 Index = 0; Index != SpawnVolume->NeedleActor.Num(); ++Index)
		{
			if (Index % 6 == 0 || Index % 6 == 5) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
			else if (Index <= 5 || Index >= 30) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
		}
		break;
	case 4:
		for (int32 Index = 0; Index != SpawnVolume->NeedleActor.Num(); ++Index)
		{
			if (Index % 6 == 2 || Index % 6 == 3) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
			else if (Index >= 12 && Index <= 23 ) {
				ANeedleActor* Needle = Cast<ANeedleActor>(SpawnVolume->NeedleActor[Index]);
				Needle->ActivateObstacle(this);
			}
		}
		break;
	}
}

void ASpartaGameState::EndWave()
{
	// Remove timer
	GetWorldTimerManager().ClearTimer(LevelTimerHadle);

	CurrentWave++;
	if (CurrentWave >= MaxWave) EndLevel();

	// Restart timer
	LevelDuration = 60.0f;
	GetWorldTimerManager().SetTimer(
		LevelTimerHadle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	// SpawnVolume 찾기
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.Num() > 0) {
		// Spawn Obstacle
		SpawnObstacle(FoundVolumes[0]);
		SpawnStartItem(FoundVolumes[0]);
	}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHadle);

	if (UGameInstance* GameInstance = GetGameInstance()) {
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance) {
			AddScore(Score);
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels) {
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex)) {
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else {
		OnGameOver();
	}
}

void ASpartaGameState::UpdateHud()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();

	if (PlayerController) {
		if (SpartaPlayerController) {
			if (HUDWidget) {
				// Set timer text
				UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TB_Time")));
				if (TimeText) {
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHadle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime < 0 ? 0 : RemainingTime)));
				}

				// Set score text
				UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TB_Score")));
				if (ScoreText) {
					if (UGameInstance* GameInstance = GetGameInstance()) {
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance) {
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				// Set level text
				UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TB_Level")));
				if (LevelIndexText) {
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex + 1)));
				}

				// Set wave text
				UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TB_Wave")));
				if (WaveIndexText) {
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), CurrentWave + 1)));
				}
			}
		}
	}
}
