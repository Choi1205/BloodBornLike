// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskRandomMoveAfterAttack.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIcontroller.h"
#include "CPPTreeEnemy.h"
#include "CPPTreeStayEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	if (Enemy != nullptr) {
		//�÷��̾ ���� ���͸� ���Ѵ�.
		FVector towardPlayer = playerLocation - Enemy->GetActorLocation();

		//�� Task�� �ݺ������� ����ȴ�. ���� ������ ������ 1���� �ٲ۴�.
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == false) {
			//�÷��̾� ����̵� ������ true��
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("MovingAfterAttack"), true);
			Enemy->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
			Enemy->GetCharacterMovement()->MaxAcceleration = 200.0f;
		}

		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == true) {
			float distance = FVector::Distance(RandomLocation, Enemy->GetActorLocation());
			if (distance < 100.0f || OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("RandomTime")) < 0) {
				AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), false);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("MovingAfterAttack"), false);
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
				Enemy->GetCharacterMovement()->MaxAcceleration = 2048.0f;
				return EBTNodeResult::Succeeded;
			}
		}
	}
	else {
		ACPPTreeStayEnemy* stayEnemy = Cast<ACPPTreeStayEnemy>(AIOwner->GetPawn());//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����

		if (stayEnemy != nullptr) {
			//�÷��̾ ���� ���͸� ���Ѵ�.
			FVector towardPlayer = playerLocation - stayEnemy->GetActorLocation();

			//�� Task�� �ݺ������� ����ȴ�. ���� ������ ������ 1���� �ٲ۴�.
			if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == false) {
				//�÷��̾� ����̵� ������ true��
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("MovingAfterAttack"), true);
				stayEnemy->GetCharacterMovement()->MaxWalkSpeed = 100.0f;
				stayEnemy->GetCharacterMovement()->MaxAcceleration = 200.0f;
			}

			if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == true) {
				float distance = FVector::Distance(RandomLocation, stayEnemy->GetActorLocation());
				if (distance < 100.0f || OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("RandomTime")) < 0)
				{
					AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("MovingAfterAttack"), false);
					stayEnemy->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
					stayEnemy->GetCharacterMovement()->MaxAcceleration = 2048.0f;
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	//������� ������ ���������� �޼� ���߰ų�, nullptr�� ���Դ�.
	return EBTNodeResult::Failed;
}