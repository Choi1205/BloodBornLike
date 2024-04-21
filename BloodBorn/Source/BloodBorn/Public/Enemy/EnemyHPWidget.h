// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UProgressBar* pb_healthBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* damageText;

	int32 damageStack = 0;

	FTimerHandle showingTimer;

	void SetHealthBar(float precentValue, float damage);

};
