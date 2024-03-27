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

	//플레이어의 위치를 구한다.
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//플레이어를 향한 백터를 구한다.
	FVector towardPlayer = playerLocation - Enemy->GetActorLocation();

	FVector RandomLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));

	//플레이어를 바라보면서 이동한다.
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