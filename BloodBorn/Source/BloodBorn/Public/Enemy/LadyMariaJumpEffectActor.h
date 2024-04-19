// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LadyMariaJumpEffectActor.generated.h"

UCLASS()
class BLOODBORN_API ALadyMariaJumpEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadyMariaJumpEffectActor();

	/// <summary>
	/// 컴포넌트 구역
	/// </summary>

	UPROPERTY(EditAnywhere)
	class USceneComponent* rootComp;
	UPROPERTY(EditAnywhere)
	class URotatingMovementComponent* rotatingComp;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordTrail1_1;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordTrail1_2;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordTrail2_1;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordTrail2_2;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordBurst1_1;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordBurst1_2;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordBurst2_1;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* swordBurst2_2;

	/// <summary>
	/// 변수 구역
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<UNiagaraComponent*> swordTrailArray;
	UPROPERTY(EditAnywhere)
	TArray<UNiagaraComponent*> swordBurstArray;

	UPROPERTY(EditAnywhere)
	float swordTrailTimer1 = 0.0f;
	UPROPERTY(EditAnywhere)
	float swordTrailTimer2 = 0.2f;

	const float swordTrailActiveTime = 0.4f;

	UPROPERTY(EditAnywhere)
	float swordBurstTimer1 = 0.0f;
	UPROPERTY(EditAnywhere)
	float swordBurstTimer2 = 0.05f;

	const float swordBurstActiveTime = 0.1f;

	bool bIsJumping = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void JumpingToggle();
	
};
