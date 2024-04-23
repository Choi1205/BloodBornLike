
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "LadyMaria.generated.h"

UENUM(BlueprintType)
enum class EPhaseState : uint8 {
	PHASE1,
	PHASE2,
	PHASE3,
};

UCLASS()
class BLOODBORN_API ALadyMaria : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ALadyMaria();

	///////////////////
	// ������Ʈ ���� //
	///////////////////

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* hair;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* rightSword1;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* rightTwoSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* rightSword2;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* rightSword3;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* leftSword1;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* leftSword2;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gun;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* rightDamageCollision;//���ݹ���

	UPROPERTY(EditAnywhere)
	class UBoxComponent* leftDamageCollision;//���ݹ���

	UPROPERTY(EditAnywhere)
	class USceneComponent* bulletFirePoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* hitEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* bloodThrustEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* jumpWindStartEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* jumpWindEndEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* jumpAttackEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* jumpAttackFireEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* phase3Effect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HoldAttackHitEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* deadBlowEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* deadCloud;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* rightEffect_V;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* rightEffect_H;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABGMActor> bgmActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALadyMariaJumpEffectActor> jumpEffect;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABloodDecalActor> bloodDecal;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* floatingWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxHealthPoint = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float healthPoint = 2000.0f;

protected:
	virtual void BeginPlay() override;

private:

	//////////////
	// �������� //
	//////////////

	float findPlayerTimer = 0.0f;

	float phase2HP = 1400.0f;

	float phase3HP = 700.0f;

	float stamina = 1000.0f;

	float staminaRegain = 50.0f;

	float stunTimer = 0.0f;

	float distanceToPlayer = 10000.0f;

	float playerSpeed = 10000.0f;

	float weakAttack = 50.0f;

	float strongAttack = 100.0f;

	FVector movePlace;

	const float moveDeltaTime = 0.276996f;
	const float assultDeltaTime = 0.490706f;

	float temp = 0.0f;

	class ALadyMariaAIController* mariaAI = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	class ABGMActor* bgmInstance;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////
	// �������� //
	//////////////
	
	//������� 1~3
	EPhaseState phaseState = EPhaseState::PHASE1;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSuperArmor = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCanParryed = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHitHoldAttack = true;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCanDealDamage = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAimmingWhileAttack = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsMovingWhileAttack = false;

	UPROPERTY(BlueprintReadOnly)
	class ABloodBornCharacter* playerREF = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ASmokeFXActor* smokeActor1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ASmokeFXActor* smokeActor2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ASmokeFXActor> smokeVFX;

	class ALadyMariaJumpEffectActor* jumpEffectInstance;

	class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimQuickSlash;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimStrongSlash;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimQuickThrust;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimChargeThrust;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimFireGun;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimAssult;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimJumpAttack;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeForward;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeLeft;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeRight;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimBossHit;

	UPROPERTY(EditAnywhere)
	class USoundBase* swordSwingSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* swordSwingSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* swordSwingSound3;

	UPROPERTY(EditAnywhere)
	class USoundBase* swordSwingSound4;

	UPROPERTY(EditAnywhere)
	class USoundBase* bloodSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* bloodSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* bloodExplodeSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireExplodeSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* dodgeSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* dodgeSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* dodgeSound3;

	UPROPERTY(EditAnywhere)
	class USoundBase* gunFireSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitVoice1;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitVoice2;

	UPROPERTY(EditAnywhere)
	class USoundBase* parryedSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitHoldAttackSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* dieSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* dieSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* dieVoice;


	//////////////
	// �Լ����� //
	//////////////

	//��� �÷��̿��� ����. �÷��̾� ���۷��� ȹ��
	class ABloodBornCharacter* FindPlayer_BP();

	float GetPlayerDistance();

	float GetPlayerSpeed();

	float GetBossStamina();

	void WalkToPlayer();

	void ForwardDodge();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GotDamage(float damage);

	UFUNCTION(BlueprintCallable)
	virtual void GotParryAttackCPP(float damage) override;

	virtual bool GetInStun() override;

	virtual bool GetHoldAttackOK() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() override;

	virtual void Lockon(bool value) override;

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Slash();

	void Thrust();

	void JumpAttack();

	void Assult();

	void AimGun();

	void EffectOn();

	void MakeBloodDecal(FVector makePlace, bool bIsForPlayer);

	void SwingSoundPlay();

	/////////////////
	// ABP�Լ����� //
	/////////////////

	void ABP_FireGun();

	void ABP_2ndSlowEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_BossHitEnd();

	void ABP_BossJumpStart();

	void ABP_BossJumpTop();

	void ABP_BossJumpLand();

	void ABP_AssultChargeEnd();

	void ABP_AssultDodgeEnd();

	void ABP_Boss_Dead();

};
