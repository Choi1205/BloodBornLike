// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BloodBornGameInstance.generated.h"

UENUM()
enum class EPlatformState : uint8{
	TOP,
	MOVING,
	BOTTOM,
};

/**
 * 
 */
UCLASS()
class BLOODBORN_API UBloodBornGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	EPlatformState platformState = EPlatformState::BOTTOM;
	bool bHadSaw = false;
	bool bHadGun = false;
};
