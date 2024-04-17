// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetVial(int32 Vial);
	void SetBullet(int32 Bullet);
	void SetHealthSliderBarPercent(float Percent);

	void SetPredictedHPBarPercent(float Percent);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* VialText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BulletText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PredictedHP;

	UPROPERTY(meta = (BindWidget))
	class USlider* HealthSliderBar;
};
