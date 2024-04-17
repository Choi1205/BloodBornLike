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

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Action State")
	EActionState ActionState;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY()
	UAttributeComponent* Attributes;

private:
	UFUNCTION()
	void AnimNotify_Heal();
};
