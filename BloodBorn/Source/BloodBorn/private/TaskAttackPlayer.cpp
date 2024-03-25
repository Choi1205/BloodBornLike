// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskAttackPlayer.h"
#include "AIcontroller.h"
#include "GameFramework/Character.h"
#include "CPPTreeEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

UTaskAttackPlayer::UTaskAttackPlayer() {
	bNotifyTick = true;
}

EBTNodeResult::Type UTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIOwner = OwnerComp.GetAIOwner();//�� �׽�ũ�� ����ϴ� AI��Ʈ�ѷ��� ������ ����

	Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());//������ ������ AI��Ʈ�ѷ��� �������� ���� ������ ����
	
	AnimInstance = Enemy->GetMesh()->GetAnimInstance();//�������� ���� �ִ� �ν��Ͻ��� ����

	if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		AnimPlaying = false;
		return EBTNodeResult::Aborted;
	}
	
	if(AnimInstance->Montage_IsPlaying(NULL)){
		//���ݸ�� ������϶�
		//UE_LOG(LogTemp, Warning, TEXT("IS waiting"));
		//return EBTNodeResult::InProgress;
	}
	else if (!AnimInstance->Montage_IsPlaying(NULL) && AnimPlaying) {
		//���ݸ���� ������� �ƴϰ�, InAttackAnim�� Ʈ��(���� �ִԿ� �������� ������ ����)�ϋ�
		//�������� �������� �ִ� ����� ������ ����
		//UE_LOG(LogTemp, Warning, TEXT("IS AnimEnd"));
		AnimPlaying = false;
		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), false);
		return EBTNodeResult::Succeeded;
	}
	else {
		AnimInstance->Montage_Play(Enemy->EnemyAttackAnimation);//�������� ���� �ִ� �ν��Ͻ��� ���Ͽ� �������� ���� ��Ÿ�ָ� ���
		//UE_LOG(LogTemp, Warning, TEXT("IS AnimPlayStart"));
		AnimPlaying = true;
		//return EBTNodeResult::InProgress;
	}

	if (AnimPlaying) {
		//UE_LOG(LogTemp, Warning, TEXT("InProgress"));
		return EBTNodeResult::InProgress;
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Completed!"));
		return EBTNodeResult::Succeeded;
	}
}

void UTaskAttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaTime);
	/*
	if (AnimInstance->Montage_IsPlaying(NULL)) {
		//���ݸ�� ������϶�
		UE_LOG(LogTemp, Warning, TEXT("waiting"));
	}
	else if (!AnimInstance->Montage_IsPlaying(NULL) && AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InAttackAnim"))) {
		//���ݸ���� ������� �ƴϰ�, InAttackAnim�� Ʈ��(���� �ִԿ� �������� ������ ����)�ϋ�
		//�������� �������� �ִ� ����� ������ ����
		//AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("InAttackAnim"), false);
		AnimPlaying = false;
		UE_LOG(LogTemp, Warning, TEXT("AnimEnd"));
	}
	else {
		//AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("InAttackAnim"), true);
		AnimPlaying = true;
		UE_LOG(LogTemp, Warning, TEXT("AnimPlayStart"));
	}
	*/
	ExecuteTask(OwnerComp, NodeMemory);
}