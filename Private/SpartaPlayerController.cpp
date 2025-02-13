// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SpartaGameState.h"

ASpartaPlayerController::ASpartaPlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr)
{
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHud()
{
    if (HUDWidgetInstance) {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance) {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (HUDWidgetClass) {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance) {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
        }

        ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
        if (SpartaGameState) {
            SpartaGameState->UpdateHud();
        }
    }
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
    if (HUDWidgetInstance) {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance) {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass) {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance) {
            MainMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }

        // Start Button text change
        UTextBlock* StartText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TB_Start")));
        if (StartText) {
            if (bIsRestart) {
                StartText->SetText(FText::FromString(TEXT("Restart")));
            }
            else {
                StartText->SetText(FText::FromString(TEXT("Start")));
            }
        }

        // Quit button text change
        UTextBlock* QuitText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TB_End")));
        if (QuitText) {
            if (bIsRestart) {
                QuitText->SetText(FText::FromString(TEXT("MainMenu")));
            }
            else {
                QuitText->SetText(FText::FromString(TEXT("QuitGame")));
            }
        }

        // GameOver UI print
        if (bIsRestart) {
            // Play GameOver text animation
            UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
            if (PlayAnimFunc) {
                MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
            }

            // Set score text
            UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("TB_TotalScore")));
            if (TotalScoreText) {
                USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this));
                if (SpartaGameInstance) {
                    TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score : %d"), SpartaGameInstance->TotalScore)));
                }
            }
        }
    }
}

void ASpartaPlayerController::StartGame()
{
    USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (SpartaGameInstance) {
        SpartaGameInstance->CurrentLevelIndex = 0;
        SpartaGameInstance->TotalScore = 0;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("IntermediateLevel"));
    SetPause(false);
}

void ASpartaPlayerController::QuitGame()
{
    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MainLevel")) {
        this->ConsoleCommand("quit");
    }
    else {
        UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
    }
}

void ASpartaPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    // 만약 레벨이 MainLevel인 경우 MainMenu UI 출력
    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MainLevel")) {
        ShowMainMenu(false);
    }
}
