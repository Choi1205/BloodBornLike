// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskRandomMoveAfterAttack.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIcontroller.h"
#include "CPPTreeEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTaskRandomMoveAfterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�� �׽�ũ�� ����ϴ� AI��Ʈ�ѷ��� ������ ����
	AAIController* AIOwner = OwnerComp.GetAIOwner();

	//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����
	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());

	//������ �ð����� �÷��̾� �ֺ��� ��Ÿ��� �ð�
	float randomTime = FMath::FRandRange(2.0f, 4.0f);
	//���� �ð��� �����忡 �Է�
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);
	//���ִ� ����Ž� ������ ȹ��
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (NavArea) {
		//�� �ڽ��� �߽����� �� 500.0f������ ���� ��ġ�� ����
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), Enemy->GetActorLocation(), RandomLocation, 15000.0f);
	}
	else {
		return EBTNodeResult::Failed;
	}

	//�÷��̾ ���� ���͸� ���Ѵ�.
	FVector towardPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - Enemy->GetActorLocation();

	//�÷��̾ �ٶ󺸸鼭 �̵��Ѵ�.
	bool result = (Enemy->SetActorLocationAndRotation(RandomLocation, towardPlayer.Rotation(), true));

	return EBTNodeResult::Succeeded;
}