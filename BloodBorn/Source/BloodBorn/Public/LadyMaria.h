
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
	class UStaticMeshComponent* leftSword;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gun;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* rightDamageCollision;//공격범위

	UPROPERTY(EditAnywhere)
	class UBoxComponent* leftDamageCollision;//공격범위

	//////////////
	// 변수구역 //
	//////////////

	class ABloodBornCharacter* playerREF = nullptr;

protected:
	virtual void BeginPlay() override;

private:

	//////////////
	// 변수구역 //
	//////////////

	float healthPoint = 2000.0f;

	float stunTimer = 0.0f;


	float distanceToPlayer = 0.0f;

	class ALadyMariaAIController* mariaAI = nullptr;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////
	// 변수구역 //
	//////////////

	//비긴 플레이에서 실행. 플레이어 레퍼런스 획득
	class ABloodBornCharacter* FindPlayer_BP();

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation1;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation2;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation3;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation4;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation5;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyHitAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyDyingAnimation;
	
	class UAnimInstance* AnimInstance;

	//////////////
	// gka수구역 //
	//////////////

	void WalkToPlayer();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GotDamage(float damage);

	UFUNCTION(BlueprintCallable)
	virtual void GotParryAttackCPP(float damage) override;
};
