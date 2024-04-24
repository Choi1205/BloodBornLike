// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "WeaponOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UWeaponOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* img_Saw;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* img_Gun;

/*	void SetSawUI();*/
};
