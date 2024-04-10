// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "NavigationSystem.h"
#include "LadyMaria.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//ƽ �̺�Ʈ ��ȿȭ
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	EnemyREF = Cast<ALadyMaria>(GetPawn());

}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), EnemyREF->GetPlayerDistance());

	float stamina = EnemyREF->GetBossStamina();

	//���ϻ��°� �ƴҶ� && ���ݸ���� ���� �����϶�
	if (!bIsStun && attackState == EAttackState::IDLE) {
		if (!bIsFireGun && EnemyREF->GetPlayerDistance() > 500.0f && !bIsDualSword) {
			bIsFireGun = true;
			EnemyREF->AimGun();
		}
		else if (EnemyREF->GetPlayerDistance() > 500.0f && EnemyREF->GetPlayerDistance() < 1000.0f && EnemyREF->phaseState != EPhaseState::PHASE1) {
			//2��������ʹ� ��Ÿ� ������ ��Ⱑ ������.
			attackState = EAttackState::THRUST;
			EnemyREF->Thrust();
		}

		if (EnemyREF->GetPlayerDistance() >= 250.0f && EnemyREF->GetPlayerDistance() < 500.0f && bIsForwardDodge && stamina > 700.0f) {
			EnemyREF->ForwardDodge();
		}
		//�÷��̾�� �Ÿ��� �ְų�, ���׹̳ʰ� �Ҹ�� ��Ȳ�ϰ��
		if (EnemyREF->GetPlayerDistance() >= 250.0f || stamina < 1000.0f) {
			EnemyREF->WalkToPlayer();
		}

		//1������, �Ÿ� 200����, ���ݰ��� ���׹̳�, �ƹ� �ൿ�� ���� �ʴ»���, �÷��̾��� �ӵ��� 100�̸��� ���
		if (EnemyREF->phaseState == EPhaseState::PHASE1 && EnemyREF->GetPlayerDistance() < 200.0f && stamina > 700.0f && attackState == EAttackState::IDLE && EnemyREF->GetPlayerSpeed() < 100.0f) {
			//�и� ī���ͷ� ��Ⱑ ������.
			attackState = EAttackState::THRUST;
			EnemyREF->Thrust();
		}

		if (EnemyREF->GetPlayerDistance() < 250.0f && stamina > 700.0f && attackState == EAttackState::IDLE) {
			if (RandomNextMoveTF(50)) {
				attackState = EAttackState::RIGHTSLASH;
			}
			else {
				attackState = EAttackState::LEFTSLASH;
			}
		}
	}

	if (bIsFireGun) {
		gunFireTimer += DeltaTime;
		if (gunFireTimer > 2.0f) {
			bIsFireGun = false;
			gunFireTimer = 0.0f;
		}
	}

	if(EnemyREF->phaseState == EPhaseState::PHASE1){
		dualSwordTimer += DeltaTime;
		if(dualSwordTimer > modeChangeTime){
			if(bIsDualSword){
				bIsDualSword = false;
			}
			else{
				bIsDualSword = true;
			}
			dualSwordTimer = 0.0f;
			bIsChangeMode = true;
		}
	}
	else {
		bIsDualSword = true;
	}
}

//���� �ൿ�� ���� �����ϴ� �Լ�
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
