// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "LongRangeEnemy.generated.h"

class UAttributeComponent;

UCLASS()
class BLOODBORN_API ALongRangeEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALongRangeEnemy();

	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gunMesh;

	UPROPERTY(EditAnywhere)
	class USceneComponent* bulletFirePoint;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiaSys;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* bleeding;

	UPROPERTY(BlueprintReadOnly)
	bool CanSeePlayer = false;

	class ABloodBornCharacter* PlayerREF;

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;

	UPROPERTY(EditAnywhere)
	float TrackingPlayer = 2.0f;

	UPROPERTY(BlueprintReadOnly)
	float AimmingTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool SetFireGun = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float health = 40.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(BlueprintReadOnly)
	bool takeHit = false;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* floatingLightComp;

	//UPROPERTY(EditAnywhere)
	//class UWidgetComponent* floatingHPComp;

	class UEnemyHPWidget* hpWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION()
	void OnHearNoise(APawn* PlayerPawn, const FVector &Location, float Volume);

	void SetCanSeePlayer(bool SeePlayer, UObject* Player);

	UFUNCTION()
	void FireBullet();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GotDamage(float damage);

	UFUNCTION(BlueprintCallable)
	virtual void GotParryAttackCPP(float damage) override;

	virtual bool GetInStun() override;

	virtual float GetHealth() override;

	virtual void Lockon(bool value) override;

private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	ABloodBornCharacter* Player;

	float lostPlayerTimer = 0.0f;

	float hitTimer = 0.0f;
};
