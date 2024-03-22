// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_GetRandomLocation_CPP.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UTask_GetRandomLocation_CPP : public UBTTaskNode
{
	GENERATED_BODY()

private:
	class UNavigationSystemV1* NavArea;

	FVector RandomLocation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
