// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodBornGameMode.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABloodBornGameMode::ABloodBornGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/76MJ/Character/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}