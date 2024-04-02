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
	//플레이어의 위치를 구한다.
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//공격 후 생성된 적 주변 랜덤위치를 가져온다
	FVector RandomLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));

	//이 테스크를 사용하는 AI컨트롤러를 변수로 저장
	AAIController* AIOwner = OwnerComp.GetAIOwner();

	//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장
	ACPPTreeEnemy* Enemy = Cast<ACPPTreeEnemy>(AIOwner->GetPawn());

	if (Enemy != nullptr) {
		//플레이어를 향한 백터를 구한다.
		FVector towardPlayer = playerLocation - Enemy->GetActorLocation();

		//이 Task는 반복적으로 실행된다. 따라서 실행중 조건은 1번만 바꾼다.
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == false) {
			//플레이어 경계이동 변수를 true로
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
		ACPPTreeStayEnemy* stayEnemy = Cast<ACPPTreeStayEnemy>(AIOwner->GetPawn());//위에서 저장한 AI컨트롤러가 조종중인 폰을 변수로 저장

		if (stayEnemy != nullptr) {
			//플레이어를 향한 백터를 구한다.
			FVector towardPlayer = playerLocation - stayEnemy->GetActorLocation();

			//이 Task는 반복적으로 실행된다. 따라서 실행중 조건은 1번만 바꾼다.
			if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == false) {
				//플레이어 경계이동 변수를 true로
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

	//여기까지 왔으면 성공조건을 달성 못했거나, nullptr이 나왔다.
	return EBTNodeResult::Failed;
}