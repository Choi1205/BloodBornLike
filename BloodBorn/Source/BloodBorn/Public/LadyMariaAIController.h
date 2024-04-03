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
	bool bIsActing = false;
	//�÷��̾� ���۷���
	class ABloodBornCharacter* playerREF;
	//��(����)���۷���
	class ACharacter* EnemyREF;

	float distanceToPlayer = 0.0f;

	class UNavigationSystemV1* NavArea;//��ã�� ����. ������Ʈ��.Build.cs ���ϵ� ������ ��

	FVector RandomLocation;//���� ������ ��ġ�� ����Ű�� ����



	//////////////
	// �Լ����� //
	//////////////

	//��� �÷��̿��� ����. �÷��̾� ���۷��� ȹ��
	class ABloodBornCharacter* FindPlayer_BP();

	void WalkToPlayer();

};
