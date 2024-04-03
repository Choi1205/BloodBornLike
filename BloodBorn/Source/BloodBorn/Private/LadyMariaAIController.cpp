// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "NavigationSystem.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//ƽ �̺�Ʈ ��ȿȭ
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	EnemyREF = Cast<ACharacter>(GetPawn());
}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}