// Fill out your copyright notice in the Description page of Project Settings.
/*
AI��Ʈ�ѷ������� ���� ĳ���͸� �۵���Ű���ʰ�, �۵��ϴ� ��ɸ� �������� �����Ͽ� �ڵ� �����
*/

#include "Enemy/LadyMariaAIController.h"
#include "Enemy/LadyMaria.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//ƽ �̺�Ʈ ��ȿȭ
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();
	//�����ϸ� �������� ������ ĳ��
	EnemyREF = Cast<ALadyMaria>(GetPawn());

}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//������ ���׹̳����� ��� üũ
	float stamina = EnemyREF->GetBossStamina();

	//���ϻ��°� �ƴҶ� && ���ݸ���� ���� �����϶�, �Ÿ��� ���� �ָ� ���. ��� ���¸� ������ �ϳ�?
	if (!bIsStun && attackState == EAttackState::IDLE && EnemyREF->GetPlayerDistance() < veryLongRange) {
		//�÷��̾�� �Ÿ��� �ֶ� �ߵ��ϴ� �ൿ���� üũ�Ѵ�.
		
		//��Ÿ� ���� ���
		//3������, ����Ÿ� ��Ȳ�϶�, ������ �����ϸ� �������� ����ض�.
		if (EnemyREF->phaseState == EPhaseState::PHASE3 && EnemyREF->GetPlayerDistance() > longRange && stamina > moveableStamina && !bIsForwardDodge) {
			bIsForwardDodge = true;
			attackState = EAttackState::FORWARDDODGE;
			EnemyREF->ForwardDodge();
		}

		//1�������� ��ü�� ���� && �÷��̾� �Ÿ��� 500 �ʰ���
		if (EnemyREF->GetPlayerDistance() > midRange && !bIsDualSword && gunFireTimer == gunFireCooldown) {
			//���� ����.
			EnemyREF->AimGun();
			gunFireTimer = 0.0f;
		}
		//2��������ʹ� ��Ÿ� ������ ��Ⱑ ������. ���� �ָ� �ߵ� ����
		else if (EnemyREF->GetPlayerDistance() > midRange && EnemyREF->GetPlayerDistance() < longRange && stamina > moveableStamina && EnemyREF->phaseState != EPhaseState::PHASE1 && chargeTimer == chargeCooldown) {
			//50%Ȯ���� �������/��������
			if (RandomNextMoveTF(50)) {
				attackState = EAttackState::CHARGETHRUST;
				chargeTimer = 0.0f;
			}
			else {
				attackState = EAttackState::CHARGESLASH;
				chargeTimer = 0.0f;
			}
		}

		//�̵� ���� ���
		//500����, 200�ʰ� �Ÿ�, ���� �ൿ ���� �� üũ���� �����ð� ON�ǰ�, ������ ���� ������ ������ ��
		if (EnemyREF->GetPlayerDistance() > closeRange && EnemyREF->GetPlayerDistance() <= midRange && bIsForwardDodge && stamina > moveableStamina) {
			//�ǰݸ��׼ǵ�, �ٸ� �ִϸ��̼��� ������� �ƴѰ��
			if (!EnemyREF->AnimInstance->IsAnyMontagePlaying()) {
				//�����ø� �ض�
				attackState = EAttackState::FORWARDDODGE;
				EnemyREF->ForwardDodge();
			}
		}
		//�� �ܿ� 200�ʰ��Ÿ��̰ų�, �ൿ ������ ���׹̳����� ������
		else if (EnemyREF->GetPlayerDistance() > closeRange || stamina <= moveableStamina) {
			//�ɾ ����/������ �¿� �̵��� �ض�
			EnemyREF->WalkToPlayer();
		}

		//�Ÿ� 200����, ���ݰ��� ���׹̳�, �÷��̾��� �ӵ��� 100�̸��� ���, ��� ��ٿ �����ٸ�
		if (EnemyREF->GetPlayerDistance() <= closeRange && stamina > moveableStamina && EnemyREF->GetPlayerSpeed() < 100.0f && chargeTimer == chargeCooldown) {
			//�и� ������ ��⸦ �ض�
			attackState = EAttackState::QUICKTHRUST;
			chargeTimer = 0.0f;
		}
		//�׿� ��Ȳ�� ���
		else if (EnemyREF->GetPlayerDistance() <= closeRange && stamina > moveableStamina) {
			//�Ϲݰ��� 2�� �ǽ�
			if (RandomNextMoveTF(50)) {
				//�̰� �и� ����
				attackState = EAttackState::QUICKSLASH;
			}
			else {
				//�̰� �и� �Ұ�
				attackState = EAttackState::STRONGSLASH;
			}
		}
		
		//���� Ÿ�̸ӵ�

		//��� ���縦 �������� Ÿ�̸�. ������ ���ڸ����� ��� �Ѹ� ���.
		if (gunFireTimer < gunFireCooldown) {
			gunFireTimer = FMath::Min(gunFireTimer + DeltaTime, gunFireCooldown);
		}
		//2������� �ְ� ����
		if (EnemyREF->phaseState == EPhaseState::PHASE2 && !bIsDualSword) {
			bIsDualSword = true;
		}
		//1������ ���� ��ü Ÿ�̸�
		else if (EnemyREF->phaseState == EPhaseState::PHASE1) {
			dualSwordTimer = FMath::Min(dualSwordTimer + DeltaTime, modeChangeTime);
			if (dualSwordTimer == modeChangeTime) {
				//bIsDualSword�� true�� �ְ�, flase�� ��ü��
				if (bIsDualSword) {
					bIsDualSword = false;
				}
				else {
					bIsDualSword = true;
				}
				dualSwordTimer = 0.0f;
				bIsChangeMode = true;
			}
		}
	}

	//���/�������� Ÿ�̸�
	if (chargeTimer < chargeCooldown) {
		chargeTimer = FMath::Min(chargeTimer + DeltaTime, chargeCooldown);
	}
}

//Ȯ���� �ʿ��Ҷ� ����Ѵ�. rate�� Ȯ���� true/false�� ����
bool ALadyMariaAIController::RandomNextMoveTF(int32 rate)
{
	int32 num = FMath::RandRange(0, 99);

	if (num < rate) {
		return true;
	}
	else {
		return false;
	}
}
