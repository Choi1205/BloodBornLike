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
	virtual void NativeUpdateAnimation(float DeltaTIime) override;  // Tickó�� ��ŸŸ�� ȣ��

	//bp�� �����ų�Ŷ� ������ public
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
