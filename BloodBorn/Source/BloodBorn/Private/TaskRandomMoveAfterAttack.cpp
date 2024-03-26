// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskRandomMoveAfterAttack.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AIcontroller.h"
#include "CPPTreeEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTaskRandomMoveAfterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//이 테스크를 사용하는 AI컨트롤러를 변수로 저장
	AAIController* AIOwner = OwnerComp.GetAIOwner();

	//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장
	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());

	//랜덤한 시간동안 플레이어 주변을 어물거리는 시간
	float randomTime = FMath::FRandRange(2.0f, 4.0f);
	//랜덤 시간을 블랙보드에 입력
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);
	//서있는 내비매쉬 범위를 획득
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (NavArea) {
		//적 자신을 중심으로 한 500.0f범위의 랜덤 위치를 지정
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), Enemy->GetActorLocation(), RandomLocation, 15000.0f);
	}
	else {
		return EBTNodeResult::Failed;
	}

	//플레이어를 향한 백터를 구한다.
	FVector towardPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - Enemy->GetActorLocation();

	//플레이어를 바라보면서 이동한다.
	bool result = (Enemy->SetActorLocationAndRotation(RandomLocation, towardPlayer.Rotation(), true));

	return EBTNodeResult::Succeeded;
}