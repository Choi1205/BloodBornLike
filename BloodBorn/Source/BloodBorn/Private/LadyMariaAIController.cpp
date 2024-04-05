// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "NavigationSystem.h"
#include "LadyMaria.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//틱 이벤트 유효화
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.

	EnemyREF = Cast<ALadyMaria>(GetPawn());

}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), EnemyREF->GetPlayerDistance());

	float stamina = EnemyREF->GetBossStamina();

	//플레이어와 거리가 멀고, 공격명령이 없는 상태일때
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

//연속 행동을 할지 결정하는 함수
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
