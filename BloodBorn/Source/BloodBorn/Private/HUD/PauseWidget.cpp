// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PauseWidget.h"
#include "Components/Button.h"
#include "BloodBornGameMode.h"
#include "EngineUtils.h"
#include "../BloodBornCharacter.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeBtn->OnClicked.AddDynamic(this, &UPauseWidget::ResumeGame);

	RestartBtn->OnClicked.AddDynamic(this, &UPauseWidget::RestartGame);

	QuitBtn->OnClicked.AddDynamic(this, &UPauseWidget::QuitGame);

	gm = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
}

void UPauseWidget::ResumeGame()
{
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
	gm->RestartGame();
}

void UPauseWidget::QuitGame()
{
	gm->EndGame();
}
