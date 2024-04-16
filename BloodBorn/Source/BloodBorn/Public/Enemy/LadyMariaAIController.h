// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LadyMariaAIController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAttackState : uint8 {
	IDLE,
	QUICKSLASH,
	STRONGSLASH,
	QUICKTHRUST,
	CHARGETHRUST,
	JUMPATTACK,
	ASSULT,
	AIMMINGGUN,
	FORWARDDODGE,
	TAKEHIT,
	DIED
};

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

	// �и��� ���ߴ°�?
	UPROPERTY(VisibleAnywhere)
	bool bIsStun = false;

	//�̵����� �ൿ�� �ϸ� �� ������ true�� �ȴ�. �ٸ� �ൿ�� �ǽ��� �� �ִ��� Ʈ���� ������ �Ѵ�.
	UPROPERTY(VisibleAnywhere)
	EAttackState attackState = EAttackState::IDLE;

	//���� ��ü �÷���. bIsChangeMode�� ���ⱳü �ִϸ��̼� ����� ���ο� �ΰ� ����� ��
	bool bIsDualSword = true;
	bool bIsChangeMode = false;

	//True�� ��ȸ��, False�� ��ȸ��
	UPROPERTY(VisibleAnywhere)
	bool bIsLeftMove = true;

	//������ ��� ���� �÷���
	UPROPERTY(VisibleAnywhere)
	bool bIsForwardDodge = false;

	//��(����)���۷���
	class ALadyMaria* EnemyREF;

	//////////////
	// �Լ����� //
	//////////////

	//���ڰ��� �־ ȣ���ϸ� �ش� ���ڰ� Ȯ���� ���� True/False�� �����Ѵ�
	bool RandomNextMoveTF(int32 rate);

private:
	//////////////
	// �������� //
	//////////////

	//�Ÿ� ���� ����. ������
	const float veryLongRange = 5000.0f;
	const float longRange = 1000.0f;
	const float midRange = 500.0f;
	const float closeRange = 200.0f;

	//���� ������ ���׹̳�. ������.
	const float moveableStamina = 700.0f;

	//���, ���������� ��ٿ�
	const float chargeCooldown = 5.0f;
	//���, �������� Ÿ�̸�. ����� ���� Ÿ�̸Ӱ� 0�� �ǰ�, ��ŸŸ���� ���ؼ� ��ٿ���� ���� ���� ����.
	float chargeTimer = 5.0f;

	//1������ ��� ���� Ÿ�̸�. ���� ���� ����
	const float modeChangeTime = 20.0f;
	float dualSwordTimer = 0.0f;
	

	//��� ��ٿ�. ���� ���� ����
	const float gunFireCooldown = 2.0f;
	float gunFireTimer = 2.0f;
};
