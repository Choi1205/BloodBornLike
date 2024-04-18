 // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "BloodBornCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAnimMontage;
class AItem;
class AWeapon;
class AGun;
class ABloodVial;
class UAttributeComponent;
class UPlayerOverlay;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABloodBornCharacter : public ACharacter, public IPickupInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GunFireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseBloodVialAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MakeBulletAction;


public:
	ABloodBornCharacter();


	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void HoldAttackStart(AActor* DamagedActor, float DamageAmount, EAttackType AttackType);


	UFUNCTION(BlueprintCallable)
	void HoldAttackEnd(AActor* DamagedActor, float DamageAmount, EAttackType AttackType);

	UFUNCTION(BlueprintCallable)
	void WeaponVisible();

	UFUNCTION(BlueprintCallable)
	void WeaponInvisible();
	
	UFUNCTION(BlueprintCallable)
	void GunVisible();

	UFUNCTION(BlueprintCallable)
	void GunInvisible();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	int32 Selection = 0;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	// virtual void AddBloodVials(ABloodVial* BloodVial) override;

	void OnSuccessfulAttack(float Damage, EAttackType AttackType);  // ������ ������ ������

	UPROPERTY(BlueprintReadOnly)
	float moveX;
	UPROPERTY(BlueprintReadOnly)
	float moveY;

	void UseBloodVial();
	void Heal();
	void Decline();

	/**
	* Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* StrongAttackMontage;	
	
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HoldAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* StepMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* BloodVialMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* MakeBulletMontage;

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


	void FKeyPressed();

	void Dodge();

	bool HasEnoughStamina(float staCost);

	void Die();

	//void Step();
	//void step();
	void LockOn();

	void GunFire();
	void MakeBullets();

	void HandleDamage(float DamageAmount);  // ,, virtual?

	// void UseBloodVial();

	/**
	* Play Montage Function
	*/
	void PlayNormalAttackMontage();

	void PlayStrongAttackMontage();

	void PlayHoldAttackMontage();

	// animmontage �� ���Ÿ� ���� �κ� �� �ϳ�~
	void PlayDodgeMontage();

	// void PlayStepAnim();

	//void PlayStepMontage();

	void PlayHitReactMontage();

	void PlayDeathMontage();

	void PlayStepMontage();

	void PlayFireMontage();

	void PlayBloodVialMontage();

	void PlayMakeBulletMontage();


	
	/*UFUNCTION(BlueprintCallable)
	void HoldAttackStart();*/


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;
	
	// float GetLastDamageReceived() const { return LastDamageReceived; }

	float LastHitTime;
	float RallyWindowDuration;
	// float LastDamageReceived;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }  // Setter Inline Func

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	// FORCEINLINE EActionState GetActionState() const { return ActionState; }

	UPROPERTY(BlueprintReadWrite)
	bool bIsInvincible = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLockOn = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsDodge = false;

	bool IsInRallyWindow() const;

	//UPROPERTY(EditAnywhere)
	//float health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mouseInputUpDown = 0;

	int32 attackType = 0;

	float staCooldown = 0.0f;

	// float ApplyDamage(AActor* DamagedActor, float DamageAmount, EAttackType AttackType);

	//���µ� ����
	class AActor* LockOnEnemyREF = nullptr;

private:
	void SetHUDHealth();

	void PlayMontageSection(UAnimMontage* Montage, const FName& sectionName);
	//void HandleDamage(float DamageAmount);

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeaponSaw;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AGun* EquippedGun;

	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	ABloodVial* EquippedBV;

	//UPROPERTY(EditAnywhere, Category = Weapon)
	//TSubclassOf<ULegacyCameraShake> CameraShake;


	//float LastAttackTime;
	//float LastDamageDealt;  // ���������� ���� ������
	// currentHealth �� Attribute���� ���� �� 

	/////////////////
	//ȯ�� �߰��κ�//
	/////////////////

	//���� ����
	class AActor* EngageLockOn();
	//���� ����
	void DisEngageLockOn();


	//���½��۽� ���� ���� ã��� & ���µ� ������ �Ÿ�
	float lockOnEnemyDistance = 800.0f;

	//��ݰ��� ������
	float gunDamage = 5.0f;
};

