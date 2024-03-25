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
	AIOwner = OwnerComp.GetAIOwner();//이 테스크를 사용하는 AI컨트롤러를 변수로 저장

	Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장
	
	AnimInstance = Enemy->GetMesh()->GetAnimInstance();//조종중인 폰의 애님 인스턴스를 저장

	if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		AnimPlaying = false;
		return EBTNodeResult::Aborted;
	}
	
	if(AnimInstance->Montage_IsPlaying(NULL)){
		//공격모션 재생중일때
		//UE_LOG(LogTemp, Warning, TEXT("IS waiting"));
		//return EBTNodeResult::InProgress;
	}
	else if (!AnimInstance->Montage_IsPlaying(NULL) && AnimPlaying) {
		//공격모션이 재생중이 아니고, InAttackAnim이 트루(공격 애님에 재생명령이 떨어진 상태)일떄
		//재생명령이 떨어지고 애님 재생이 끝나면 실행
		//UE_LOG(LogTemp, Warning, TEXT("IS AnimEnd"));
		AnimPlaying = false;
		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), false);
		return EBTNodeResult::Succeeded;
	}
	else {
		AnimInstance->Montage_Play(Enemy->EnemyAttackAnimation);//조종중인 폰의 애님 인스턴스를 통하여 조종중인 폰의 몽타주를 재생
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
		//공격모션 재생중일때
		UE_LOG(LogTemp, Warning, TEXT("waiting"));
	}
	else if (!AnimInstance->Montage_IsPlaying(NULL) && AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InAttackAnim"))) {
		//공격모션이 재생중이 아니고, InAttackAnim이 트루(공격 애님에 재생명령이 떨어진 상태)일떄
		//재생명령이 떨어지고 애님 재생이 끝나면 실행
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