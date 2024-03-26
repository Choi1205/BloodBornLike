// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageSystem.h"
#include "Interfaces/HitInterface.h"
#include "CPPTreeEnemy.generated.h"

class UAttributeComponent;

UCLASS()
class BLOODBORN_API ACPPTreeEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPTreeEnemy();

	//bool PlayerDetected;//플레이어 발견시 true
	//bool CanAttackPlayer;//플레이어가 공격행동 범위에 들어올 시 true

	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;//블루프린트에서도 수정 가능. 플레이어가 공격 범위에 들어올 시 true

	UPROPERTY(BlueprintReadWrite)
	bool CanParryed;//블루프린트에서도 수정 가능. 페리 가능 타이밍일때 true

	class ABloodBornCharacter* PlayerREF;//플레이어 캐릭터를 저장. 클래스명은 A+프로젝트명+Charactor
	/*
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerCollisionDetection;//어디서든 수정 가능. 플레이어 탐지범위
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerAttackCollisionDetection;//공격행동 범위
	*/
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;//공격범위
	
	class ABTAIController* BTAIController;//AI컨트롤러의 기능을 사용하기 위해 컨트롤러에 대한 참조가 필요하다.
	/*
	void OnAIMoveCompleted(struct FAIRequestID RequsetID, const struct FPathFollowingResult& Result);//AI가 이동을 끝냈을때 알려줌.

	UPROPERTY(EditAnywhere)
	float StoppingDistance = 100.0f;//플레이어와 100.0f위치에 도착하면 멈춘다. 매쉬 충돌문제로 0거리로 오려고하면 이상해진다.

	FTimerHandle SeekPlayerTimerHandle;//플레이어 탐색시 일정시간마다 연속으로 탐색 명령을 내리는데, 그 시간을 컨트롤하는 핸들러 변수
	
	UFUNCTION()
	void MoveToPlayer();//플레이어에게 이동

	UFUNCTION()
	void SeekPlayer();//플레이어 위치를 지속적으로 갱신

	UFUNCTION()
	void StopSeekingPlayer();//플레이어 위치 갱신을 중단, 랜덤이동으로 전환

	UFUNCTION()
	void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//플레이어가 탐지범위에 들어왔을때. 플레이어 추적을 시작함
	UFUNCTION()
	void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//플레이어가 탐지범위를 빠져나갔을때. 랜덤이동으로 전환함
	UFUNCTION()
	void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//플레이어가 공격 가능범위에 들어왔을 때. 이동을 멈추고 공격 행동으로 전환함
	UFUNCTION()
	void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//플레이어가 공격 가능 범위를 벗어났을 떄. 플레이어 추적을 시작함.
	*/
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
	/*
	UFUNCTION(BlueprintCallable)
	void AttackAnimationEnded();
	*/
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
private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
};
