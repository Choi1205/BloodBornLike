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

	//�÷��̾�� �Ÿ��� �ְ�, ���ݸ���� ���� �����϶�
	if (EnemyREF->GetPlayerDistance() > 150.0f && EnemyREF->bIsActing == false) {
		EnemyREF->WalkToPlayer();
		attackTimer = 0.0f;
	}

	if (EnemyREF->GetPlayerDistance() <= 150.0f && EnemyREF->bIsActing == false) {
		UE_LOG(LogTemp, Warning, TEXT("Start"));
		bIsRightSlash = true;
		EnemyREF->bIsActing = true;
	}
}