// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RandomMoveEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ARandomMoveEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

private:
	class UNavigationSystemV1* NavArea;//길찾기 범위. 프로젝트명.Build.cs 파일도 참조할 것

	FVector RandomLocation;//다음 랜덤한 위치를 가리키는 백터

public:
	UFUNCTION()
		void RandomPatrol();//랜덤이동 함수

};
