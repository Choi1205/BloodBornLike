// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LadyMariaAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ALadyMariaAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ALadyMariaAIController();

	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//////////////
	// 변수구역 //
	//////////////

	//이동외의 행동을 하면 이 변수가 true가 된다. 다른 행동을 실시할 수 있는지 트리거 역할을 한다.
	bool bIsRightSlash = false;
	bool bIsLeftSlash = false;

	//페이즈는 1~4(숫자는 현재 페이즈 - 1)
	int32 phase = 0;

	//적(보스)레퍼런스
	class ALadyMaria* EnemyREF;

	class UNavigationSystemV1* NavArea;//길찾기 범위. 프로젝트명.Build.cs 파일도 참조할 것

	//////////////
	// 함수구역 //
	//////////////

	bool RandomNextMoveTF(int32 rate);

private:
	//////////////
	// 변수구역 //
	//////////////
	float attackTimer = 0.0f;


};
