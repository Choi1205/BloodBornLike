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
	QUICKSLASH,
	STRONGSLASH,
	QUICKTHRUST,
	CHARGETHRUST,
	JUMPATTACK,
	ASSULT,
	AIMMINGGUN,
	FORWARDDODGE,
	TAKEHIT,
	DIED
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
	UPROPERTY(VisibleAnywhere)
	bool bIsStun = false;

	//이동외의 행동을 하면 이 변수가 true가 된다. 다른 행동을 실시할 수 있는지 트리거 역할을 한다.
	UPROPERTY(VisibleAnywhere)
	EAttackState attackState = EAttackState::IDLE;

	//무기 교체 플래그. bIsChangeMode는 무기교체 애니메이션 재생을 염두에 두고 만들어 둠
	bool bIsDualSword = true;
	bool bIsChangeMode = false;

	//True면 우회전, False면 좌회전
	UPROPERTY(VisibleAnywhere)
	bool bIsLeftMove = true;

	//전방대시 사용 예정 플래그
	UPROPERTY(VisibleAnywhere)
	bool bIsForwardDodge = false;

	//적(보스)레퍼런스
	class ALadyMaria* EnemyREF;

	//////////////
	// 함수구역 //
	//////////////

	//인자값을 넣어서 호출하면 해당 인자값 확율에 따라 True/False를 리턴한다
	bool RandomNextMoveTF(int32 rate);

private:
	//////////////
	// 변수구역 //
	//////////////

	//거리 기준 변수. 조정용
	const float veryLongRange = 5000.0f;
	const float longRange = 1000.0f;
	const float midRange = 500.0f;
	const float closeRange = 200.0f;

	//공격 가능한 스테미나. 조정용.
	const float moveableStamina = 700.0f;

	//찌르기, 차지베기의 쿨다운
	const float chargeCooldown = 5.0f;
	//찌르기, 차지베기 타이머. 기술을 쓰면 타이머가 0이 되고, 델타타임을 더해서 쿨다운까지 차면 재사용 가능.
	float chargeTimer = 5.0f;

	//1페이즈 모드 변경 타이머. 위와 같은 원리
	const float modeChangeTime = 20.0f;
	float dualSwordTimer = 0.0f;
	

	//사격 쿨다운. 위와 같은 원리
	const float gunFireCooldown = 2.0f;
	float gunFireTimer = 2.0f;
};
