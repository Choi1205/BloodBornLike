// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"

void UPlayerOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
		UE_LOG(LogTemp, Warning, TEXT("myhealth:%f"), Percent);
	}
}

void UPlayerOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetVial(int32 Vial)
{
	if (VialText)
	{
		const FString VString = FString::Printf(TEXT("%d"), Vial);
		const FText VText = FText::FromString(VString);
		VialText->SetText(VText);
	}
}

void UPlayerOverlay::SetBullet(int32 Bullet)
{
	if (BulletText)
	{
		BulletText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Bullet)));
	}
}


void UPlayerOverlay::SetPredictedHPBarPercent(float Percent)
{
	if (PredictedHP)
	{
		PredictedHP->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetDecreaseStaminaBarPercent(float Percent)
{
	if (DecreaseStaminaBar)
	{
		DecreaseStaminaBar->SetPercent(Percent);
	}
}
// 
// void UPlayerOverlay::ShowDiedWidget(bool bshow)
// {
// 
// }

void UPlayerOverlay::SetHealthSliderBarPercent(float Percent)
{
	if (HealthSliderBar)
	{
		HealthSliderBar->SetValue(Percent);
	}
}

void UPlayerOverlay::SetStaminaSliderBarPercent(float Percent)
{
	if (StaminaSliderBar)
	{
		StaminaSliderBar->SetValue(Percent);
	}
}

