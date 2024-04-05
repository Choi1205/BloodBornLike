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

	//�÷��̾�� �Ÿ��� �ְ�, ���ݸ���� ���� �����϶�
	if (EnemyREF->GetPlayerDistance() > 150.0f && EnemyREF->bIsActing == false) {
		EnemyREF->WalkToPlayer();
	}

	if (EnemyREF->GetPlayerDistance() <= 150.0f && EnemyREF->bIsActing == false && stamina == 1000.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Start"));
		bIsRightSlash = true;
		EnemyREF->bIsActing = true;
	}
	else if(EnemyREF->GetPlayerDistance() <= 150.0f && EnemyREF->bIsActing == false && stamina < 1000.0f) {
		EnemyREF->WalkToPlayer();
	}
}

//���� �ൿ�� ���� �����ϴ� �Լ�
bool ALadyMariaAIController::RandomNextMoveTF(int32 rate)
{
	int32 num = FMath::RandRange(0, 99);

	if(EnemyREF->GetPlayerDistance() > 150.0f){
		return false;
	}

	if (num < rate) {
		return true;
	}
	else {
		return false;
	}
}
