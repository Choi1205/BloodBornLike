
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
	// ������Ʈ ���� //
	///////////////////

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* rightSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* leftSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gun;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* rightDamageCollision;//���ݹ���

	UPROPERTY(EditAnywhere)
	class UBoxComponent* leftDamageCollision;//���ݹ���

	//////////////
	// �������� //
	//////////////

	class ABloodBornCharacter* playerREF = nullptr;

protected:
	virtual void BeginPlay() override;

private:

	//////////////
	// �������� //
	//////////////

	float healthPoint = 2000.0f;

	float stamina = 1000.0f;

	float stunTimer = 0.0f;

	float distanceToPlayer = 10000.0f;

	class ALadyMariaAIController* mariaAI = nullptr;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////
	// �������� //
	//////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAimmingWhileAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMovingWhileAttack = false;

	//��� �÷��̿��� ����. �÷��̾� ���۷��� ȹ��
	class ABloodBornCharacter* FindPlayer_BP();

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
	class UAnimMontage* AnimBossHit;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AnimBossDying;

	//////////////
	// �Լ����� //
	//////////////

	void WalkToPlayer();

	float GetPlayerDistance();

	float GetBossStamina();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GotDamage(float damage);

	UFUNCTION(BlueprintCallable)
	virtual void GotParryAttackCPP(float damage) override;

	void RightSlash();

	void LeftSlash();
};
