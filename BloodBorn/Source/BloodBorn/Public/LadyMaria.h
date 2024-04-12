
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
	class UNiagaraComponent* instanceEffect;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* rightEffect;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* leftEffect;


	//////////////
	// �������� //
	//////////////

protected:
	virtual void BeginPlay() override;

private:

	//////////////
	// �������� //
	//////////////

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
	// �������� //
	//////////////

	//������� 1~3
	EPhaseState phaseState = EPhaseState::PHASE1;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSuperArmor = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCanParryed = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsStun = false;

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
	class UAnimMontage* AnimRightSlash;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimLeftSlash;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDualThrust;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimGunShot;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimQuickSlash;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeForward;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeLeft;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimDodgeRight;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimBossHit;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimBossDying;

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

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() override;

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RightSlash();

	void LeftSlash();

	void Thrust();

	void AimGun();

	/////////////////
	// ABP�Լ����� //
	/////////////////

	UFUNCTION(BlueprintCallable)
	void FireGun();

	UFUNCTION(BlueprintCallable)
	void ABP_AttackStart();

	UFUNCTION(BlueprintCallable)
	void ABP_SlowEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_ThrustSlowEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_AttackEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_DodgeStart();

	UFUNCTION(BlueprintCallable)
	void ABP_DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_BossHitEnd();
};
