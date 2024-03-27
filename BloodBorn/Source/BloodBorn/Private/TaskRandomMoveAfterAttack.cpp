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

	//�÷��̾��� ��ġ�� ���Ѵ�.
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//�÷��̾ ���� ���͸� ���Ѵ�.
	FVector towardPlayer = playerLocation - Enemy->GetActorLocation();

	FVector RandomLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));

	//�÷��̾ �ٶ󺸸鼭 �̵��Ѵ�.
	Enemy->SetActorLocationAndRotation(RandomLocation, towardPlayer.Rotation(), true);

	float distance = FVector::Distance(RandomLocation, Enemy->GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("%f"), distance);
	if (distance > 100.0f) {
		UE_LOG(LogTemp, Warning, TEXT("failed : %f"), distance);
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Succeed : %f"), distance);
		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), false);
		return EBTNodeResult::Succeeded;
	}

	
}