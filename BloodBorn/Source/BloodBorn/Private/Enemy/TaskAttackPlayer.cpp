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
	AAIController* AIOwner = OwnerComp.GetAIOwner();//이 테스크를 사용하는 AI컨트롤러를 변수로 저장

	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장
	if (Enemy) {
		UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();//조종중인 폰의 애님 인스턴스를 저장

		//실행 중 패링이 발생하는 등 다른 사건이 일어난 경우 중단처리
		if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			return EBTNodeResult::Aborted;
		}

		//조종중인 폰의 애님 인스턴스를 통하여 조종중인 폰의 몽타주를 재생
		AnimInstance->Montage_Play(Enemy->EnemyAttackAnimation);

		//랜덤한 시간동안 플레이어 주변을 어물거리는 시간
		float randomTime = FMath::FRandRange(2.0f, 4.0f);
		//랜덤 시간을 블랙보드에 입력
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);

		//서있는 내비매쉬 범위를 획득
		UNavigationSystemV1* NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (NavArea) {
			FVector RandomLocation;
			//적 자신을 중심으로 한 1500.0f범위의 랜덤 위치를 지정
			NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), Enemy->GetActorLocation(), RandomLocation, 400.0f);
			//Z축 높이를 현 위치와 같은 높이로.
			RandomLocation.Z = Enemy->GetActorLocation().Z;
			//랜덤 패트롤 위치로 지정
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);
		}
		else {
			return EBTNodeResult::Failed;
		}

		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), true);
	}
	else {
		ACPPTreeStayEnemy* stayEnemy = Cast<ACPPTreeStayEnemy>(AIOwner->GetPawn());//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장

		UAnimInstance* AnimInstance = stayEnemy->GetMesh()->GetAnimInstance();//조종중인 폰의 애님 인스턴스를 저장

		//실행 중 패링이 발생하는 등 다른 사건이 일어난 경우 중단처리
		if (!AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) || AIOwner->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			return EBTNodeResult::Aborted;
		}

		//조종중인 폰의 애님 인스턴스를 통하여 조종중인 폰의 몽타주를 재생
		AnimInstance->Montage_Play(stayEnemy->EnemyAttackAnimation);

		//랜덤한 시간동안 플레이어 주변을 어물거리는 시간
		float randomTime = FMath::FRandRange(2.0f, 4.0f);
		//랜덤 시간을 블랙보드에 입력
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);

		//서있는 내비매쉬 범위를 획득
		UNavigationSystemV1* NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (NavArea) {
			FVector RandomLocation;
			//적 자신을 중심으로 한 1500.0f범위의 랜덤 위치를 지정
			NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), stayEnemy->GetActorLocation(), RandomLocation, 400.0f);
			//Z축 높이를 현 위치와 같은 높이로.
			RandomLocation.Z = stayEnemy->GetActorLocation().Z;
			//랜덤 패트롤 위치로 지정
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("RandomPatrolLocation"), RandomLocation);
		}
		else {
			return EBTNodeResult::Failed;
		}

		AIOwner->GetBlackboardComponent()->SetValueAsBool(FName("AttackedPlayer"), true);
	}
	
	

	return EBTNodeResult::Succeeded;
}