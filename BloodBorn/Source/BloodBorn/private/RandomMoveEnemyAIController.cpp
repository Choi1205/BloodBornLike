// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMoveEnemyAIController.h"
#include "NavigationSystem.h"

void ARandomMoveEnemyAIController::BeginPlay() {

	Super::BeginPlay();//이거 없으면 안움직임. 상속받은 BeginPlay를 그대로 가져오고, 그 밑에 명령을 추가하고 있다.
	//현재 BeginPlay를 OverRide하고 있으므로, 이게 없으면 상속받은 BeginPlay가 없어진다.

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.

	RandomPatrol();//랜덤이동을 바로 실시
}

void ARandomMoveEnemyAIController::RandomPatrol() {
	if (NavArea) {//Navi매쉬 범위 안에 있는경우
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//내비매쉬 범위 안에서 5000.0f범위안의 랜덤한 지점을 RandomLocation에 입력한다.

		MoveToLocation(RandomLocation);//위에서 입력된 RandomLocation으로 이동한다.
	}
}