// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BloodBornGameMode.generated.h"

UCLASS(minimalapi)
class ABloodBornGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ABloodBornGameMode();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPauseWidget> pauseWidget_bp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDieOverlay> clearOverlay;

	class UPauseWidget* pauseWidget;

	UFUNCTION(BlueprintCallable)
	void ShowPauseUI();
	void HidePauseUI();
	void ShowClearUI();
};



