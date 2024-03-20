// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GetRandomLocation_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTask_GetRandomLocation_CPP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (NavArea) {
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), RandomLocation, 15000.0f);
	}
	else {
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);

	return EBTNodeResult::Succeeded;
}
