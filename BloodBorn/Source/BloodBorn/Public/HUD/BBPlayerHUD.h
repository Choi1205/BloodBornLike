// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BBPlayerHUD.generated.h"


class UPlayerOverlay;
class UDieOverlay;

UCLASS()
class BLOODBORN_API ABBPlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;
	// virtual void BeginPlay() override;

	
private:
	UPROPERTY(EditDefaultsOnly, Category = BBPlayer)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY(EditDefaultsOnly, Category = BBPlayer)
	TSubclassOf<UDieOverlay> DieOverlayClass;

	UPROPERTY()
	UDieOverlay* DieOverlay;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }
	FORCEINLINE UDieOverlay* GetDieOverlay() const { return DieOverlay; }

	void ShowDieOverlay();
};
