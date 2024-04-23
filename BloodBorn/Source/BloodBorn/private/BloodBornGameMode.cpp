// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodBornGameMode.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HUD/DieOverlay.h"
#include "HUD/PauseWidget.h"
#include "Kismet/GameplayStatics.h"

void ABloodBornGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (pauseWidget_bp != nullptr) {
		pauseWidget = CreateWidget<UPauseWidget>(GetWorld(), pauseWidget_bp);
		if (pauseWidget != nullptr) {
			pauseWidget->AddToViewport();
			pauseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}

ABloodBornGameMode::ABloodBornGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/76MJ/Character/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABloodBornGameMode::ShowPauseUI()
{
	pauseWidget->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UGameplayStatics::PlaySound2D(GetWorld(), openMenu);
}

void ABloodBornGameMode::HidePauseUI()
{
	pauseWidget->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::PlaySound2D(GetWorld(), closeMenu);
}

void ABloodBornGameMode::ShowClearUI()
{
	if (clearOverlay != nullptr) {
		UDieOverlay* clearOverlayUI = CreateWidget<UDieOverlay>(GetWorld(), clearOverlay);
		if (clearOverlayUI != nullptr) {
			clearOverlayUI->AddToViewport();
			clearOverlayUI->PlayAnimation(clearOverlayUI->anim_died);
		}
	}
}

void ABloodBornGameMode::RestartGame()
{
	UGameplayStatics::PlaySound2D(GetWorld(), OKBtn);
	UGameplayStatics::OpenLevel(GetWorld(), FName("DemoMapDay_Copy"));
}

void ABloodBornGameMode::EndGame()
{
	UGameplayStatics::PlaySound2D(GetWorld(), OKBtn);
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}
