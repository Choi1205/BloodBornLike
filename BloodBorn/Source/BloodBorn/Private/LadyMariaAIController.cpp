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

	UE_LOG(LogTemp, Warning, TEXT("%f"), EnemyREF->GetPlayerDistance());

	float stamina = EnemyREF->GetBossStamina();


	//���ݸ���� ���� �����϶�
	if (EnemyREF->bIsActing == false) {
		if (EnemyREF->GetPlayerDistance() > 250.0f && EnemyREF->GetPlayerDistance() < 300.0f && bIsForwardDodge) {
			EnemyREF->ForwardDodge();
		}
		//�÷��̾�� �Ÿ��� �ְų�, ���׹̳ʰ� �Ҹ�� ��Ȳ�ϰ��
		if (EnemyREF->GetPlayerDistance() > 150.0f || stamina < 1000.0f) {
			EnemyREF->WalkToPlayer();
		}

		if (EnemyREF->GetPlayerDistance() <= 150.0f && stamina == 1000.0f) {
			UE_LOG(LogTemp, Warning, TEXT("Start"));
			bIsRightSlash = true;
			EnemyREF->bIsActing = true;
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
