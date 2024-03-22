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
	//�������� �޾����� ȣ���
	virtual void GotDamagedCPP(float Damage) = 0;
	//��ݰ����� �޾��� �� ȣ���. ȣ�� �� GotDamaged�� ȣ���ؾ� ��
	virtual void GotParryAttackCPP(float Damage) = 0;
	//=0�� �ȳ����� �۵��� ���Ѵ�.
};
