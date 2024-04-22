// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodBornGameMode.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HUD/DieOverlay.h"

ABloodBornGameMode::ABloodBornGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/76MJ/Character/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABloodBornGameMode::ShowClearUI()
{
	if (clearOverlay != nullptr) {
		UDieOverlay* clearOverlayUI = CreateWidget<UDieOverlay>(GetWorld(), clearOverlay);
		if (clearOverlayUI != nullptr) {
			clearOverlayUI->AddToViewport();
			clearOverlayUI->PlayAnimation(clearOverlayUI->anim_died);

			/*FTimerHandle clearUITimer;
			GetWorld()->GetTimerManager().SetTimer(clearUITimer, FTimerDelegate::CreateLambda([&]() {
				clearOverlayUI->Destruct();
				}), 5.0f, false);*/
		}
	}
}
