// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	playerREF = FindPlayer_BP();
}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	distanceToPlayer = FVector::Distance(GetPawn()->GetActorLocation(), playerREF->GetActorLocation());
}

ABloodBornCharacter* ALadyMariaAIController::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer���·� ��ȯ�Ѵ�.
	return Cast<ABloodBornCharacter>(players[0]);
}

void ALadyMariaAIController::RandomMove()
{
	if (NavArea) {//Navi�Ž� ���� �ȿ� �ִ°��
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//����Ž� ���� �ȿ��� 5000.0f�������� ������ ������ RandomLocation�� �Է��Ѵ�.

		MoveToLocation(RandomLocation);//������ �Էµ� RandomLocation���� �̵��Ѵ�.
	}
}