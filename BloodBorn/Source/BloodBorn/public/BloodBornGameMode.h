// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BloodBornGameMode.generated.h"

UCLASS(minimalapi)
class ABloodBornGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABloodBornGameMode();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDieOverlay> clearOverlay;

	void ShowClearUI();
};



