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
class UDieOverlay;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;


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

	void OnSuccessfulAttack(float Damage, EAttackType AttackType);  // 적에게 입히는 데미지

	UPROPERTY(BlueprintReadOnly)
	float moveX;
	UPROPERTY(BlueprintReadOnly)
	float moveY;

	void UseBloodVial();
	void Heal();
	void Decline();

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* lockonUI;

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
	UAnimMontage* HeavyHitReactMontage;

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

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* regainHealEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* healEffect;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* makeBulletEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* finishBulletEffect;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* deadEffect;

// 	UPROPERTY(EditAnywhere)
// 	class UNiagaraComponent* instanceEffect;

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

	void ShowMenu();

	void HandleDamage(float DamageAmount);  // ,, virtual?

	// void UseBloodVial();

	/**
	* Play Montage Function
	*/
	void PlayNormalAttackMontage();

	void PlayStrongAttackMontage();

	void PlayHoldAttackMontage();

	// animmontage 안 쓸거면 지울 부분 중 하나~
	void PlayDodgeMontage();

	// void PlayStepAnim();

	//void PlayStepMontage();

	void PlayHitReactMontage();

	void PlayHeavyHitReactMontage();

	void PlayDeathMontage();

	void PlayStepMontage();

	void PlayFireMontage();

	void PlayBloodVialMontage();

	void PlayMakeBulletMontage();


	
	/*UFUNCTION(BlueprintCallable)
	void HoldAttackStart();*/
// 	void OpenLevelFunction();

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

	//락온된 액터
	class AActor* LockOnEnemyREF = nullptr;


	//카메라 작업부분
	float cameraZoomInAnimTime = 0.1f;
	float cameraZoomInTimer = 0.0f;
	float cameraZoomOutAnimTime = 0.5f;
	float cameraZoomOutTimer = 0.0f;

	float zoomInInt = 1;

	float startFOV = 90.0f;
	float endFOV = 70.0f;

	bool bIsZoomIn = false;
	bool bIsZoomOut = false;

	//메뉴화면 카메라 고정 변수
	bool bIsShowingMenu = false;

	void CameraZoomInStart(float deltaTime);
	void CameraZoomOutStart(float deltaTime);

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

	UPROPERTY()
	UDieOverlay* DieOverlay;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeaponSaw;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponSaw;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AGun* EquippedGun;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AGun> WeaponGun;

	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	ABloodVial* EquippedBV;

	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* hitSound1;

	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* hitSound2;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* deathSound;
		
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* regainSound;
			
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* heavyHitSound;

	//UPROPERTY(EditAnywhere, Category = Weapon)
	//TSubclassOf<ULegacyCameraShake> CameraShake;


	//float LastAttackTime;
	//float LastDamageDealt;  // 마지막으로 가한 데미지
	// currentHealth 는 Attribute에서 받을 거 

	/////////////////
	//환록 추가부분//
	/////////////////

	//락온 시작
	class AActor* EngageLockOn();
	//락온 해제
	void DisEngageLockOn();


	//락온시작시 락온 범위 찾기용 & 락온된 적과의 거리
	float lockOnEnemyDistance = 800.0f;

	//사격공격 데미지
	float gunDamage = 5.0f;
};

