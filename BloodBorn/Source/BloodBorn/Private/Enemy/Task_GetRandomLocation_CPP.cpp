// Fill out your copyright notice in the Description page of Project Settings.
//비헤이비어 트리에서 사용하는 랜덤위치 탐색 테스크

#include "Enemy/Task_GetRandomLocation_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTask_GetRandomLocation_CPP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//이 비헤이비어 트리를 사용하는 액터와 현재 서있는 네비게이션 매쉬를 가져옴
	APawn* owner = OwnerComp.GetAIOwner()->GetPawn();
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(owner);

	//네비게이션 매쉬위에 있느면 owner를 중심으로 5000거리의 랜덤한 위치를 RandomLocation에 저장
	if (NavArea) {
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), owner->GetActorLocation(), RandomLocation, 5000.0f);
	}
	else {
		return EBTNodeResult::Failed;
	}

	//블랙보드에 입력
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);

	return EBTNodeResult::Succeeded;
}
