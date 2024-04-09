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
	if (!bIsStun && EnemyREF->bIsActing == false) {
		if (!bIsFireGun && EnemyREF->GetPlayerDistance() > 500.0f && !bIsDualSword) {
			bIsFireGun = true;
			EnemyREF->AimGun();
		}

		if (EnemyREF->GetPlayerDistance() >= 250.0f && EnemyREF->GetPlayerDistance() < 500.0f && bIsForwardDodge && stamina > 700.0f) {
			EnemyREF->ForwardDodge();
		}
		//�÷��̾�� �Ÿ��� �ְų�, ���׹̳ʰ� �Ҹ�� ��Ȳ�ϰ��
		if (EnemyREF->GetPlayerDistance() >= 250.0f || stamina < 1000.0f) {
			EnemyREF->WalkToPlayer();
		}

		if (EnemyREF->GetPlayerDistance() < 250.0f && stamina > 700.0f) {
			if (EnemyREF->GetPlayerSpeed() < 100.0f) {
				bIsThrust = true;
				//EnemyREF->bIsActing = true;
				EnemyREF->Thrust();
			}
			else if(!bIsRightSlash && !bIsLeftSlash){
				if (RandomNextMoveTF(50)) {
					bIsRightSlash = true;
					//EnemyREF->bIsActing = true;
				}
				else {
					bIsLeftSlash = true;
					//EnemyREF->bIsActing = true;
				}
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

	if(EnemyREF->phase == 0){
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
