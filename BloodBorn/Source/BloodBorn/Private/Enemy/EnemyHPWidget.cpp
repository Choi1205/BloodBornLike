// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyHPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UEnemyHPWidget::SetHealthBar(float precentValue, float damage)
{
	damageStack += (int32)damage;

	FString totaldamage = FString::FromInt(damageStack);

	pb_healthBar->SetPercent(precentValue);
	sl_healthBar->SetValue(precentValue);
	damageText->SetText(FText::FromString(totaldamage));
	damageText->SetVisibility(ESlateVisibility::Visible);

	if (GetWorld()->GetTimerManager().IsTimerActive(showingTimer))	{
		GetWorld()->GetTimerManager().ClearTimer(showingTimer);
	}

	GetWorld()->GetTimerManager().SetTimer(showingTimer, FTimerDelegate::CreateLambda([&](){
		damageText->SetVisibility(ESlateVisibility::Hidden);
		damageStack = 0.0f;
		}), 5.0f, false);
}
