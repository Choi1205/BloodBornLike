// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMoveEnemyAIController.h"
#include "NavigationSystem.h"

void ARandomMoveEnemyAIController::BeginPlay() {

	Super::BeginPlay();//�̰� ������ �ȿ�����. ��ӹ��� BeginPlay�� �״�� ��������, �� �ؿ� ����� �߰��ϰ� �ִ�.
	//���� BeginPlay�� OverRide�ϰ� �����Ƿ�, �̰� ������ ��ӹ��� BeginPlay�� ��������.

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	RandomPatrol();//�����̵��� �ٷ� �ǽ�
}

void ARandomMoveEnemyAIController::RandomPatrol() {
	if (NavArea) {//Navi�Ž� ���� �ȿ� �ִ°��
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//����Ž� ���� �ȿ��� 5000.0f�������� ������ ������ RandomLocation�� �Է��Ѵ�.

		MoveToLocation(RandomLocation);//������ �Էµ� RandomLocation���� �̵��Ѵ�.
	}
}