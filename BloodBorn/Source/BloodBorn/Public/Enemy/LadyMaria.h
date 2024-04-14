
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
	// 컴포넌트 구역 //
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
	class UBoxComponent* rightDamageCollision;//공격범위

	UPROPERTY(EditAnywhere)
	class UBoxComponent* leftDamageCollision;//공격범위

	UPROPERTY(EditAnywhere)
	class USceneComponent* bulletFirePoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* hitEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* bloodThrustEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* instanceEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* rightEffect_V;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* rightEffect_H;


	//////////////
	// 변수구역 //
	//////////////

protected:
	virtual void BeginPlay() override;

private:

	//////////////
	// 변수구역 //
	//////////////

	float findPlayerTimer = 0.0f;

	float healthPoint = 2000.0f;

	float phase2HP = 1400.0f;

	float phase3HP = 700.0f;

	float stamina = 1000.0f;

	float staminaRegain = 50.0f;

	float stunTimer = 0.0f;

	float distanceToPlayer = 10000.0f;

	float playerSpeed = 10000.0f;

	float weakAttack = 50.0f;

	float strongAttack = 100.0f;

	class ALadyMariaAIController* mariaAI = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////
	// 변수구역 //
	//////////////

	//페이즈는 1~3
	EPhaseState phaseState = EPhaseState::PHASE1;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSuperArmor = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCanParryed = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHitHoldAttack = false;

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
	class UAnimMontage* AnimGunShot;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimChargeSlash;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeForward;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeLeft;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeRight;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimBossHit;

	//////////////
	// 함수구역 //
	//////////////

	//비긴 플레이에서 실행. 플레이어 레퍼런스 획득
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

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() override;

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Slash();

	void Thrust();

	void ChargeSlash();

	void AimGun();

	void EffectMove();

	void EffectOn();

	/////////////////
	// ABP함수구역 //
	/////////////////

	UFUNCTION(BlueprintCallable)
	void ABP_FireGun();

	UFUNCTION(BlueprintCallable)
	void ABP_2ndSlowEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_BossHitEnd();
};
