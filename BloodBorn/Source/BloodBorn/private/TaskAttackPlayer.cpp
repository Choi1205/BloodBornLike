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
	AAIController* AIOwner = OwnerComp.GetAIOwner();//이 테스크를 사용하는 AI컨트롤러를 변수로 저장

	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장
	
	UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();//조종중인 폰의 애님 인스턴스를 저장

	//실행 중 패링이 발생하는 등 다른 사건이 일어난 경우 중단처리
	if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		return EBTNodeResult::Aborted;
	}
	
	//조종중인 폰의 애님 인스턴스를 통하여 조종중인 폰의 몽타주를 재생
	AnimInstance->Montage_Play(Enemy->EnemyAttackAnimation);
	
	return EBTNodeResult::Succeeded;
}