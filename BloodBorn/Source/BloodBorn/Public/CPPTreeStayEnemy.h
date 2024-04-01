// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "CPPTreeStayEnemy.generated.h"
class UAttributeComponent;

UCLASS()
class BLOODBORN_API ACPPTreeStayEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPTreeStayEnemy();

	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;//블루프린트에서도 수정 가능. 플레이어가 공격 범위에 들어올 시 true

	UPROPERTY(BlueprintReadWrite)
	bool CanParryed;//블루프린트에서도 수정 가능. 페리 가능 타이밍일때 true

	class ABloodBornCharacter* PlayerREF;//플레이어 캐릭터를 저장. 클래스명은 A+프로젝트명+Charactor

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;//공격범위

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiaSys;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* bleeding;

	class ABTStayAIController* BTStayAIController;//AI컨트롤러의 기능을 사용하기 위해 컨트롤러에 대한 참조가 필요하다.

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//플레이어가 공격 가능 범위에 있고, 공격 행동의 충돌판정에 맞았을때. 데미지를 입어야 하나, 지금은 메세지만 출력함.

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyDyingAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyHitAnimation;

	UPROPERTY(EditAnywhere)
	float healthPoint = 1000.0f;

	float stunTimer = 0.0f;

	class UAnimInstance* AnimInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DyingAnimEnd();

	UFUNCTION(BlueprintCallable)
	void HitAnimEnd();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GotDamage(float damage);

	UFUNCTION(BlueprintCallable)
	virtual void GotParryAttackCPP(float damage) override;

private:
	void AfterAttackMoving(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere)
	float Damage = 20.0f;

};
