// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodBornGameMode.h"
#include "BloodBornCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABloodBornGameMode::ABloodBornGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
