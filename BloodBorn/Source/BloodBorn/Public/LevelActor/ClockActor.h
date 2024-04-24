// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClockActor.generated.h"

UCLASS()
class BLOODBORN_API AClockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClockActor();

	UPROPERTY(EditAnywhere)
	class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clockCase;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clockFront;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clockInner;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clockBack;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clockBlad1;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* clockBlad2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRotateStart = false;

	UFUNCTION(BlueprintCallable)
	void MoveTickTok();

private:

	FTimerHandle tickTokHandle;

	float rotRate = 0.0f;
	float targetRot = 9.0f;
	float prevFrontRot;
	float nextFrontRot;
	float prevBackRot;
	float nextBackRot;

	int32 tickTokCounter = 0;
	float tickTokRate = 0.0f;
	float prevTickTokRot;
	float nextTickTokRot;
	bool bIsTickToking = false;

	float prevBlad1Rot;
	float nextBlad1Rot;
	float prevBlad2Rot;
	float nextBlad2Rot;
};
