// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RandomMoveEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ARandomMoveEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

private:
	class UNavigationSystemV1* NavArea;//��ã�� ����. ������Ʈ��.Build.cs ���ϵ� ������ ��

	FVector RandomLocation;//���� ������ ��ġ�� ����Ű�� ����

public:
	UFUNCTION()
		void RandomPatrol();//�����̵� �Լ�

};
