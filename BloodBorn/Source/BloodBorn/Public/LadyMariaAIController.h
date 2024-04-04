// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LadyMariaAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ALadyMariaAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ALadyMariaAIController();

	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//////////////
	// �������� //
	//////////////

	//�̵����� �ൿ�� �ϸ� �� ������ true�� �ȴ�. �ٸ� �ൿ�� �ǽ��� �� �ִ��� Ʈ���� ������ �Ѵ�.
	bool bIsRightSlash = false;
	bool bIsLeftSlash = false;

	//������� 1~4(���ڴ� ���� ������ - 1)
	int32 phase = 0;

	//��(����)���۷���
	class ALadyMaria* EnemyREF;

	class UNavigationSystemV1* NavArea;//��ã�� ����. ������Ʈ��.Build.cs ���ϵ� ������ ��

	//////////////
	// �Լ����� //
	//////////////

	bool RandomNextMoveTF(int32 rate);

private:
	//////////////
	// �������� //
	//////////////
	float attackTimer = 0.0f;


};
