// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UTaskAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
