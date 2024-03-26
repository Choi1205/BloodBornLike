// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskAttackPlayer.h"
#include "AIcontroller.h"
#include "GameFramework/Character.h"
#include "CPPTreeEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "BTAIController.h"

EBTNodeResult::Type UTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();//�� �׽�ũ�� ����ϴ� AI��Ʈ�ѷ��� ������ ����

	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����
	
	UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();//�������� ���� �ִ� �ν��Ͻ��� ����

	//���� �� �и��� �߻��ϴ� �� �ٸ� ����� �Ͼ ��� �ߴ�ó��
	if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		return EBTNodeResult::Aborted;
	}
	
	//�������� ���� �ִ� �ν��Ͻ��� ���Ͽ� �������� ���� ��Ÿ�ָ� ���
	AnimInstance->Montage_Play(Enemy->EnemyAttackAnimation);
	
	return EBTNodeResult::Succeeded;
}