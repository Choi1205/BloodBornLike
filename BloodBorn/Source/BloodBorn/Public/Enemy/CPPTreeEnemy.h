// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;//�������Ʈ������ ���� ����. �÷��̾ ���� ������ ���� �� true

	UPROPERTY(BlueprintReadWrite)
	bool CanParryed;//�������Ʈ������ ���� ����. �丮 ���� Ÿ�̹��϶� true

	class ABloodBornCharacter* PlayerREF;//�÷��̾� ĳ���͸� ����. Ŭ�������� A+������Ʈ��+Charactor
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* weaponMesh;//���� �Ž�

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;//���ݹ���
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiaSys;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* bleeding;

	class ABTAIController* BTAIController;//AI��Ʈ�ѷ��� ����� ����ϱ� ���� ��Ʈ�ѷ��� ���� ������ �ʿ��ϴ�.
	
	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//�÷��̾ ���� ���� ������ �ְ�, ���� �ൿ�� �浹������ �¾�����. �������� �Ծ�� �ϳ�, ������ �޼����� �����.

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation2;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyDyingAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyHitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxHealth = 40.0f;

	UPROPERTY(EditAnywhere)
	float healthPoint = 60.0f;

	float stunTimer = 0.0f;

	class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* floatingLightComp;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* floatingHPComp;

	class UEnemyHPWidget* hpWidget;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitSound2;

	UPROPERTY(EditAnywhere)
	class USoundBase* parryedSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* hitHoldAttackSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* dieSound1;

	UPROPERTY(EditAnywhere)
	class USoundBase* dieSound2;

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

	virtual bool GetInStun() override;

	virtual bool GetHoldAttackOK() override;

	virtual float GetHealth() override;

	virtual void Lockon(bool value) override;

private:
	void AfterAttackMoving(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere)
	float Damage = 20.0f;

	bool bIsLockedOn = false;

	FTimerHandle showingTimer;

};
