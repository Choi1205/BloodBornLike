#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BloodBorn/CharacterTypes.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public: 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTIime) override;  // Tick처럼 델타타임 호출

	//bp에 노출시킬거라 여전히 public
	UPROPERTY(BlueprintReadOnly)
	class ABloodBornCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	float Direction;


	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Action State")
	EActionState ActionState;

private:
	UFUNCTION()
	void AnimNotify_Heal();
	UFUNCTION()
	void AnimNotify_Decline();
	UFUNCTION()
	void AnimNotify_AttackEnd();
	UFUNCTION()
	void AnimNotify_EnableBoxCollision();
	UFUNCTION()
	void AnimNotify_DisableCollision();
	UFUNCTION()
	void AnimNotify_HitReactEnd();
	UFUNCTION()
	void AnimNotify_EnableInvincible();
	UFUNCTION()
	void AnimNotify_DisableInvincible();
	UFUNCTION()
	void AnimNotify_WeaponVisible();
	UFUNCTION()
	void AnimNotify_GunInvisible();
	UFUNCTION()
	void AnimNotify_GunVisible();
	UFUNCTION()
	void AnimNotify_WeaponInvisible();
	UFUNCTION()
	void AnimNotify_HoldAttackStart();
	UFUNCTION()
	void AnimNotify_HoldAttackFinish();
	UFUNCTION()
	void AnimNotify_RaiseArm();
	UFUNCTION()
	void AnimNotify_ReturnArm();
	UFUNCTION()
	void AnimNotify_Stay();
	UFUNCTION()
	void AnimNotify_BulletStay();
	UFUNCTION()
	void AnimNotify_Back();
	UFUNCTION()
	void AnimNotify_BulletBack();
};
