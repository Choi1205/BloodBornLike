// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossAnimInstance.h"
#include "Enemy/LadyMaria.h"
#include "Enemy/LadyMariaAIController.h"
#include "Enemy/SmokeFXActor.h"
#include "Enemy/LadyMariaJumpEffectActor.h"
#include "BloodBornGameMode.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mariaREF = Cast<ALadyMaria>(TryGetPawnOwner());
}

void UBossAnimInstance::NativeBeginPlay()
{
	if (mariaREF != nullptr) {
		mariaAIREF = Cast<ALadyMariaAIController>(mariaREF->GetController());
	}
	health = mariaREF->GetHealth();
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//현재 방향과 이동속도를 이용해서 전/후진, 좌우이동 체크. 로코모션에 사용
	if (mariaREF != nullptr) {
		FVector currentVelocity = mariaREF->GetVelocity();
		FRotator currentRot = mariaREF->GetActorRotation();
		
		FVector newVector = (currentRot.UnrotateVector(currentVelocity)).GetSafeNormal();
		moveX = newVector.X;
		moveY = newVector.Y;
		if (mariaAIREF != nullptr) {
			bIsStun = mariaAIREF->bIsStun;
		}
		bIsHitHoldAttack = mariaREF->bIsHitHoldAttack;
		health = mariaREF->GetHealth();
	}
	else {
		mariaREF = Cast<ALadyMaria>(TryGetPawnOwner());
	}
}

//공격 준비 시작
void UBossAnimInstance::AnimNotify_Boss_AttackStart()
{
	//애니메이션에서 손목을 안꺽어주므로 칼을 찌르는 순간에만 돌린다.
	//각 애니메이션의 공격준비부분의 재생속도가 너무 빠르므로 적당히 감속한다.
	if (mariaAIREF->attackState == EAttackState::QUICKTHRUST || mariaAIREF->attackState == EAttackState::CHARGETHRUST) {
		mariaREF->rightSword1->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->rightSword2->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->rightSword3->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->rightTwoSword->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->bIsSuperArmor = true;

		if (mariaAIREF->attackState == EAttackState::QUICKTHRUST) {
			Montage_SetPlayRate(NULL, 0.3f);
		}
		else if (mariaAIREF->attackState == EAttackState::CHARGETHRUST) {
			Montage_SetPlayRate(NULL, 0.2f);
		}
	}
	else if (mariaAIREF->attackState == EAttackState::QUICKSLASH || mariaAIREF->attackState == EAttackState::ASSULT) {
		Montage_SetPlayRate(NULL, 0.1f);
	}
	else if (mariaAIREF->attackState == EAttackState::STRONGSLASH || mariaAIREF->attackState == EAttackState::JUMPATTACK) {
		Montage_SetPlayRate(NULL, 0.5f);
	}
	
	//애니메이션은 공중으로 뜨지만, 루트모션이 아니라서 판정은 지상에 있으므로, 슈퍼아머판정을 넣는다.
	if (mariaAIREF->attackState == EAttackState::JUMPATTACK) {
		mariaREF->bIsSuperArmor = true;
		//점프 이펙트도 켠다.
		mariaREF->jumpEffectInstance->JumpingToggle();
	}

	//공격 준비중에는 플레이어를 추적한다.
	mariaREF->bIsAimmingWhileAttack = true;
}

//공격 준비 끝. 공격 시작.
void UBossAnimInstance::AnimNotify_Boss_SlowEnd()
{
	if (mariaAIREF->attackState == EAttackState::QUICKTHRUST) {
		Montage_SetPlayRate(mariaREF->AnimQuickThrust, 2.0f);
	}
	else if (mariaAIREF->attackState == EAttackState::CHARGETHRUST) {
		Montage_SetPlayRate(mariaREF->AnimChargeThrust, 5.0f);
	}
	else {
		Montage_SetPlayRate(NULL, 1.0f);
	}

	if (mariaREF->phaseState != EPhaseState::PHASE1) {
		//공격 이펙트 생성
		mariaREF->EffectOn();
	}

	mariaREF->SwingSoundPlay();
	//무기 판정을 켜고, 조준 추적을 끈다.
	mariaREF->bIsCanDealDamage = true;
	mariaREF->bIsAimmingWhileAttack = false;
}

//중거리 찌르기, 점프공격용. 공격 판정이 발생한다
void UBossAnimInstance::AnimNotify_Boss_2ndSlowEnd()
{
	mariaREF->ABP_2ndSlowEnd();
}

//공격판정 끝
void UBossAnimInstance::AnimNotify_Boss_AttackEnd()
{
	//견제하면서 이동은 좌우 랜덤
	if (mariaAIREF->RandomNextMoveTF(50)) {
		mariaAIREF->bIsLeftMove = true;
	}
	else {
		mariaAIREF->bIsLeftMove = false;
	}

	//전방대시 가능성은 좀 낮게
	if (mariaAIREF->RandomNextMoveTF(30)) {
		mariaAIREF->bIsForwardDodge = true;
		UE_LOG(LogTemp, Warning, TEXT("ForwardDodge is True"));
	}
	else {
		mariaAIREF->bIsForwardDodge = false;
		UE_LOG(LogTemp, Warning, TEXT("ForwardDodge is False"));
	}

	mariaAIREF->attackState = EAttackState::IDLE;
	mariaREF->bIsSuperArmor = false;
}

//공격 애니메이션이 루트모션이 아니라 수동으로 옮겨줘야한다.
void UBossAnimInstance::AnimNotify_Boss_MovingAttackStart()
{
	mariaREF->bIsMovingWhileAttack = true;
}

//공격시 전진 끝
void UBossAnimInstance::AnimNotify_Boss_MovingAttackEnd()
{
	mariaREF->bIsMovingWhileAttack = false;
	mariaREF->bIsCanDealDamage = false;
	mariaREF->assultMoving = true;
	if (mariaREF->phaseState != EPhaseState::PHASE1 && mariaAIREF->attackState == EAttackState::STRONGSLASH) {
		mariaREF->MakeBloodDecal(mariaREF->GetActorLocation(), false);
	}
}

//플레이어 사격공격 회피시 실행
void UBossAnimInstance::AnimNotify_Boss_DodgeStart()
{
	mariaREF->bIsSuperArmor = true;
	if (mariaREF->smokeActor1 != nullptr) {
		mariaREF->GetMesh()->SetVisibility(false, true);
		mariaREF->smokeActor1->SetActorLocation(mariaREF->GetActorLocation());
		mariaREF->smokeActor1->PlayFX();
	}
}

//회피 끝
void UBossAnimInstance::AnimNotify_Boss_DodgeEnd()
{
	mariaREF->ABP_DodgeEnd();
}

//보스 피격 애니메이션 끝
void UBossAnimInstance::AnimNotify_Boss_HitEnd()
{
	mariaAIREF->attackState = EAttackState::IDLE;
}

//보스 탄환이 발사되는 순간
void UBossAnimInstance::AnimNotify_Boss_GunFire()
{
	mariaREF->ABP_FireGun();
}

//보스 패리 당한 후 앞잡기 끝날 때
void UBossAnimInstance::AnimNotify_Boss_HitHoldAttackEnd()
{
	mariaREF->bIsHitHoldAttack = true;
	mariaAIREF->bIsStun = false;
	mariaAIREF->attackState = EAttackState::IDLE;
}

//패리공격 가능 플래그 온
void UBossAnimInstance::AnimNotify_Boss_ParryStart()
{
	mariaREF->bIsCanParryed = true;
}

//패리공격 가능 플래그 오프
void UBossAnimInstance::AnimNotify_Boss_ParryEnd()
{
	mariaREF->bIsCanParryed = false;
}

//패리 당하고 쓰러지는 애니메이션 종료
void UBossAnimInstance::AnimNotify_Boss_InStunEnd()
{
	mariaAIREF->bIsStun = false;
	mariaAIREF->attackState = EAttackState::IDLE;
}

//위로 뛰어오르는 순간
void UBossAnimInstance::AnimNotify_Boss_JumpStart()
{
	//이펙트 켜기
	mariaREF->ABP_BossJumpStart();
}

//점프 최대치일때
void UBossAnimInstance::AnimNotify_Boss_JumpTop()
{
	//플레이어 위치를 이용, 착지위치 계산
	mariaREF->ABP_BossJumpTop();
	Montage_SetPlayRate(NULL, 1.0f);
}

//점프 착지시
void UBossAnimInstance::AnimNotify_Boss_JumpLand()
{
	//착지 이펙트 켜기, 데미지 판정
	mariaREF->ABP_BossJumpLand();
}

//돌진공격 시작. 플레이어 우측으로 움직인다
void UBossAnimInstance::AnimNotify_Boss_AssultChargeEnd()
{
	Montage_SetPlayRate(NULL, 1.0f);
	mariaREF->ABP_AssultChargeEnd();
}

//돌진공격 판정 시작. 플레이어에게 달려든다
void UBossAnimInstance::AnimNotify_Boss_AssultDodgeEnd()
{
	mariaREF->ABP_AssultDodgeEnd();
}

//보스 사망 애니메이션 중 쓰러지는 순간
void UBossAnimInstance::AnimNotify_Boss_Dead()
{
	ABloodBornGameMode* gameMode = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
	gameMode->ShowClearUI();
	mariaREF->ABP_Boss_Dead();
}
