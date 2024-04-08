
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "LadyMaria.generated.h"

class UAttributeComponent;

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
	class UStaticMeshComponent* rightSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* rightReverseSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* leftSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gun;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* rightDamageCollision;//공격범위

	UPROPERTY(EditAnywhere)
	class UBoxComponent* leftDamageCollision;//공격범위

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiaSys;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* bleeding;

	UPROPERTY(EditAnywhere)
	class USceneComponent* bulletFirePoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	//////////////
	// 변수구역 //
	//////////////
	UPROPERTY(BlueprintReadOnly)
	class ABloodBornCharacter* playerREF = nullptr;

protected:
	virtual void BeginPlay() override;

private:

	//////////////
	// 변수구역 //
	//////////////

	float healthPoint = 2000.0f;

	float stamina = 1000.0f;

	float stunTimer = 0.0f;

	float distanceToPlayer = 10000.0f;

	float playerSpeed = 10000.0f;

	class ALadyMariaAIController* mariaAI = nullptr;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////
	// 변수구역 //
	//////////////

	//페이즈는 1~3(숫자는 현재 페이즈 - 1)
	int32 phase = 0;
	bool phase2Cheaker = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActing = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSuperArmor = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAimmingWhileAttack = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsMovingWhileAttack = false;

	//비긴 플레이에서 실행. 플레이어 레퍼런스 획득
	class ABloodBornCharacter* FindPlayer_BP();

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
	class UAnimMontage* AnimEnemyAttack5;

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
	// 함수구역 //
	//////////////

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

	void RightSlash();

	void LeftSlash();

	void Thrust();

	void AimGun();

	/////////////////
	// ABP함수구역 //
	/////////////////

	UFUNCTION(BlueprintCallable)
	void FireGun();

	UFUNCTION(BlueprintCallable)
	void ABP_AttackStart();

	UFUNCTION(BlueprintCallable)
	void ABP_SlowEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_AttackEnd();

	UFUNCTION(BlueprintCallable)
	void ABP_DodgeStart();

	UFUNCTION(BlueprintCallable)
	void ABP_DodgeEnd();
};
