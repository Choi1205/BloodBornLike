// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLOODBORN_API IDamageSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//데미지를 받았을때 호출됨
	virtual void GotDamagedCPP(float Damage) = 0;
	//사격공격을 받았을 때 호출됨. 호출 후 GotDamaged를 호출해야 함
	virtual void GotParryAttackCPP(float Damage) = 0;
	//=0을 안넣으면 작동을 안한다.
};
