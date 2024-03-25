// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterTypes.h"
#include "BloodBornCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAnimMontage;
class AItem;
class AWeapon;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABloodBornCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Normal Attack Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StrongAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

public:
	ABloodBornCharacter();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// UPROPERTY(VisibleAnywhere, Category=SawMesh)
	// class USkeletalMeshComponent* sawMeshComp;
	
	int32 Selection = 0;

protected:

	/** 
	* Callbacks for input 
	*/
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void NormalAttack();

	void StrongAttack();

	void Equip();

	void Dodge();

	/**
	* Play Montage Function
	*/
	void PlayNormalAttackMontage();

	void PlayStrongAttackMontage();

	// animmontage 안 쓸거면 지울 부분 중 하나~
	void PlayDodgeMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }  // Setter Inline Func

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	// FORCEINLINE EActionState GetActionState() const { return ActionState; }

private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& sectionName);

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	/**
	* Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* StrongAttackMontage;	
	
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;


};

