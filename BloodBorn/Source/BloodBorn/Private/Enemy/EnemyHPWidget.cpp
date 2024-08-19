// Fill out your copyright notice in the Description page of Project Settings.
//일반 에너미의 머리 위 HP바 UI


#include "Enemy/EnemyHPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

//데미지를 받으면 표시. 현재 체력비율과 받은 데미지량이 표시됨. 데미지량은 5초 안에 데미지를 더 입으면 누적되서 표시됨.
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
