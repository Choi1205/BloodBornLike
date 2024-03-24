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
	
public:
	UTaskAttackPlayer();

	class UAnimInstance* AnimInstance;

	class AAIController* AIOwner;

	class ACPPTreeEnemy* Enemy;

	bool AnimPlaying = false;

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime);

};
