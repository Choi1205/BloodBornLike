// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DieOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UDieOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_died;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_diedBack;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_black;

};
