// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LadyMariaAIController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAttackState : uint8 {
	IDLE,
	RIGHTSLASH,
	LEFTSLASH,
	THRUST,
	QUICKSLASH,
	AIMMINGGUN,
	FORWARDDODGE,
	TAKEHIT
};

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

	// 패링을 당했는가?
	bool bIsStun = false;

	//이동외의 행동을 하면 이 변수가 true가 된다. 다른 행동을 실시할 수 있는지 트리거 역할을 한다.
	UPROPERTY(VisibleAnywhere)
	EAttackState attackState = EAttackState::IDLE;
	UPROPERTY(VisibleAnywhere)
	bool bIsRightSlash = false;
	UPROPERTY(VisibleAnywhere)
	bool bIsLeftSlash = false;
	UPROPERTY(VisibleAnywhere)
	bool bIsThrust = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsFireGun = false;
	float gunFireTimer = 0.0f;

	//1페이즈 무기교체 타이머. bIsChangeMode는 무기교체 애니메이션 재생을 염두에 두고 만들어 둠
	bool bIsChangeMode = false;
	bool bIsDualSword = true;
	float dualSwordTimer = 0.0f;
	float modeChangeTime = 20.0f;

	//True면 우회전, False면 좌회전
	UPROPERTY(BlueprintReadWrite)
	bool bIsLeftMove = true;

	UPROPERTY(BlueprintReadWrite)
	bool bIsForwardDodge = false;

	//적(보스)레퍼런스
	class ALadyMaria* EnemyREF;

	class UNavigationSystemV1* NavArea;//길찾기 범위. 프로젝트명.Build.cs 파일도 참조할 것

	//////////////
	// 함수구역 //
	//////////////

	UFUNCTION(BlueprintCallable)
	bool RandomNextMoveTF(int32 rate);

private:
	//////////////
	// 변수구역 //
	//////////////
	float attackTimer = 0.0f;


};
