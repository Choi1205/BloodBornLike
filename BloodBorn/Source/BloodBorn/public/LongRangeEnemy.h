// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageSystem.h"
#include "LongRangeEnemy.generated.h"

UCLASS()
class BLOODBORN_API ALongRangeEnemy : public ACharacter, public IDamageSystem
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALongRangeEnemy();

	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	class USceneComponent* bulletFirePoint;

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

	UPROPERTY(BlueprintReadOnly)
	float health = 500.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditAnywhere)
	float healthPoint = 500.0f;

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

	void SetCanSeePlayer(bool SeePlayer, UObject* Player);

	void RunRetriggerableTimer();

	UFUNCTION()
	void FireBullet();

	UFUNCTION(BlueprintCallable)
	virtual void GotDamagedCPP(float Damage) override;
	UFUNCTION(BlueprintCallable)
	virtual void GotParryAttackCPP(float Damage) override;

};
