// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	float moveX;
	UPROPERTY(BlueprintReadOnly)
	float moveY;
	UPROPERTY(BlueprintReadOnly)
	bool bIsStun;
	UPROPERTY(BlueprintReadOnly)
	bool bIsHitHoldAttack;
	UPROPERTY(BlueprintReadOnly)
	float health = 2000.0f;

private:

	class ALadyMaria* mariaREF = nullptr;
	class ALadyMariaAIController* mariaAIREF = nullptr;

	UFUNCTION()
	void AnimNotify_Boss_AttackStart();
	UFUNCTION()
	void AnimNotify_Boss_SlowEnd();
	UFUNCTION()
	void AnimNotify_Boss_2ndSlowEnd();
	UFUNCTION()
	void AnimNotify_Boss_AttackEnd();
	UFUNCTION()
	void AnimNotify_Boss_MovingAttackStart();
	UFUNCTION()
	void AnimNotify_Boss_MovingAttackEnd();
	UFUNCTION()
	void AnimNotify_Boss_DodgeStart();
	UFUNCTION()
	void AnimNotify_Boss_DodgeEnd();
	UFUNCTION()
	void AnimNotify_Boss_HitEnd();
	UFUNCTION()
	void AnimNotify_Boss_GunFire();
	UFUNCTION()
	void AnimNotify_Boss_HitHoldAttackEnd();
	UFUNCTION()
	void AnimNotify_Boss_ParryStart();
	UFUNCTION()
	void AnimNotify_Boss_ParryEnd();
	UFUNCTION()
	void AnimNotify_Boss_InStunEnd();
	UFUNCTION()
	void AnimNotify_Boss_JumpStart();
	UFUNCTION()
	void AnimNotify_Boss_JumpTop();
	UFUNCTION()
	void AnimNotify_Boss_JumpLand();
	UFUNCTION()
	void AnimNotify_Boss_AssultChargeEnd();
	UFUNCTION()
	void AnimNotify_Boss_AssultDodgeEnd();

};
