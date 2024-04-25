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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWeaponOverlay> weaponOverlay;

	class UPauseWidget* pauseWidget;

	UPROPERTY()
	class UWeaponOverlay* weaponOverlayUI;

	UPROPERTY(EditAnywhere)
	class USoundBase* openMenu;

	UPROPERTY(EditAnywhere)
	class USoundBase* closeMenu;

	UPROPERTY(EditAnywhere)
	class USoundBase* OKBtn;

	void ShowPauseUI();
	void HidePauseUI();
	void ShowClearUI();
	void RestartGame();
	void EndGame();
	void ShowWeaponUI(bool bIsSaw);
	void HideWeaponUI();
};



