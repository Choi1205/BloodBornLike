// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TaskAttackPlayer.h"
#include "AIcontroller.h"
#include "GameFramework/Character.h"
#include "Enemy/CPPTreeEnemy.h"
#include "Enemy/CPPTreeStayEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();//�� �׽�ũ�� ����ϴ� AI��Ʈ�ѷ��� ������ ����

	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����
	if (Enemy) {
		UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();//�������� ���� �ִ� �ν��Ͻ��� ����

		//���� �� �и��� �߻��ϴ� �� �ٸ� ����� �Ͼ ��� �ߴ�ó��
		if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			return EBTNodeResult::Aborted;
		}

		//�������� ���� �ִ� �ν��Ͻ��� ���Ͽ� �������� ���� ��Ÿ�ָ� ���
		AnimInstance->Montage_Play(Enemy->EnemyAttackAnimation);

		//������ �ð����� �÷��̾� �ֺ��� ��Ÿ��� �ð�
		float randomTime = FMath::FRandRange(2.0f, 4.0f);
		//���� �ð��� �����忡 �Է�
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);

		//���ִ� ����Ž� ������ ȹ��
		UNavigationSystemV1* NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (NavArea) {
			FVector RandomLocation;
			//�� �ڽ��� �߽����� �� 1500.0f������ ���� ��ġ�� ����
			NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), Enemy->GetActorLocation(), RandomLocation, 400.0f);
			//Z�� ���̸� �� ��ġ�� ���� ���̷�.
			RandomLocation.Z = Enemy->GetActorLocation().Z;
			//���� ��Ʈ�� ��ġ�� ����
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);
		}
		else {
			return EBTNodeResult::Failed;
		}

		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), true);
	}
	else {
		ACPPTreeStayEnemy* stayEnemy = Cast<ACPPTreeStayEnemy>(AIOwner->GetPawn());//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����

		UAnimInstance* AnimInstance = stayEnemy->GetMesh()->GetAnimInstance();//�������� ���� �ִ� �ν��Ͻ��� ����

		//���� �� �и��� �߻��ϴ� �� �ٸ� ����� �Ͼ ��� �ߴ�ó��
		if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			return EBTNodeResult::Aborted;
		}

		//�������� ���� �ִ� �ν��Ͻ��� ���Ͽ� �������� ���� ��Ÿ�ָ� ���
		AnimInstance->Montage_Play(stayEnemy->EnemyAttackAnimation);

		//������ �ð����� �÷��̾� �ֺ��� ��Ÿ��� �ð�
		float randomTime = FMath::FRandRange(2.0f, 4.0f);
		//���� �ð��� �����忡 �Է�
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);

		//���ִ� ����Ž� ������ ȹ��
		UNavigationSystemV1* NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (NavArea) {
			FVector RandomLocation;
			//�� �ڽ��� �߽����� �� 1500.0f������ ���� ��ġ�� ����
			NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), stayEnemy->GetActorLocation(), RandomLocation, 400.0f);
			//Z�� ���̸� �� ��ġ�� ���� ���̷�.
			RandomLocation.Z = stayEnemy->GetActorLocation().Z;
			//���� ��Ʈ�� ��ġ�� ����
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);
		}
		else {
			return EBTNodeResult::Failed;
		}

		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), true);
	}
	
	

	return EBTNodeResult::Succeeded;
}