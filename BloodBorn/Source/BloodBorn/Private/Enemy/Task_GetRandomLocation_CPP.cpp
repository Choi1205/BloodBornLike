// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task_GetRandomLocation_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTask_GetRandomLocation_CPP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* owner = OwnerComp.GetAIOwner()->GetPawn();
	//NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(owner);

	if (NavArea) {
		//NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), RandomLocation, 15000.0f);
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), owner->GetActorLocation(), RandomLocation, 5000.0f);
	}
	else {
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);

	return EBTNodeResult::Succeeded;
}
