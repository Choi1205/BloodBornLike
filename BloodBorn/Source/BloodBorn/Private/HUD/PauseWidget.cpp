// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "BloodBornGameMode.h"
#include "../BloodBornCharacter.h"
#include "EngineUtils.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeBtn->OnClicked.AddDynamic(this, &UPauseWidget::ResumeGame);

	RestartBtn->OnClicked.AddDynamic(this, &UPauseWidget::RestartGame);

	QuitBtn->OnClicked.AddDynamic(this, &UPauseWidget::QuitGame);
}

void UPauseWidget::ResumeGame()
{
	ABloodBornGameMode* gm = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
	gm->HidePauseUI();

	ABloodBornCharacter* player = nullptr;
	for (TActorIterator<ABloodBornCharacter> iter(GetWorld()); iter; ++iter) {
		player = *iter;
		break;
	}
	if (player != nullptr) {
		player->bIsShowingMenu = false;
	}
}

void UPauseWidget::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("DemoMapDay_Copy"));
}

void UPauseWidget::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}
