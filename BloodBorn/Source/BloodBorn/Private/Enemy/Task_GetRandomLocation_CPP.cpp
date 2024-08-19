// Fill out your copyright notice in the Description page of Project Settings.
//�����̺�� Ʈ������ ����ϴ� ������ġ Ž�� �׽�ũ

#include "Enemy/Task_GetRandomLocation_CPP.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTask_GetRandomLocation_CPP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�� �����̺�� Ʈ���� ����ϴ� ���Ϳ� ���� ���ִ� �׺���̼� �Ž��� ������
	APawn* owner = OwnerComp.GetAIOwner()->GetPawn();
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(owner);

	//�׺���̼� �Ž����� �ִ��� owner�� �߽����� 5000�Ÿ��� ������ ��ġ�� RandomLocation�� ����
	if (NavArea) {
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), owner->GetActorLocation(), RandomLocation, 5000.0f);
	}
	else {
		return EBTNodeResult::Failed;
	}

	//�����忡 �Է�
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);

	return EBTNodeResult::Succeeded;
}
