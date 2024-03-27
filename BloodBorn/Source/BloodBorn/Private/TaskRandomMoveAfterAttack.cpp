// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskRandomMoveAfterAttack.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIcontroller.h"
#include "CPPTreeEnemy.h"
#include "CPPTreeStayEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTaskRandomMoveAfterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//�÷��̾��� ��ġ�� ���Ѵ�.
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//���� �� ������ �� �ֺ� ������ġ�� �����´�
	FVector RandomLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));

	//�� �׽�ũ�� ����ϴ� AI��Ʈ�ѷ��� ������ ����
	AAIController* AIOwner = OwnerComp.GetAIOwner();

	//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����
	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());

	if (Enemy) {
		//�÷��̾ ���� ���͸� ���Ѵ�.
		FVector towardPlayer = playerLocation - Enemy->GetActorLocation();

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
	else {
		ACPPTreeStayEnemy* stayEnemy = Cast<ACPPTreeStayEnemy>(AIOwner->GetPawn());//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����

		//�÷��̾ ���� ���͸� ���Ѵ�.
		FVector towardPlayer = playerLocation - stayEnemy->GetActorLocation();

		//�÷��̾ �ٶ󺸸鼭 �̵��Ѵ�.
		stayEnemy->SetActorLocationAndRotation(RandomLocation, towardPlayer.Rotation(), true);

		float distance = FVector::Distance(RandomLocation, stayEnemy->GetActorLocation());

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
	

	
}