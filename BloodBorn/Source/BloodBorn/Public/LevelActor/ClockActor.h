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
	USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* clockCase;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* clockFront;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* clockInner;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* clockBack;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* clockBlad1;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* clockBlad2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveTickTok();

private:

	FTimerHandle tickTokHandle;

	float timeCounter = 0.0f;
	float frontRotRate = 0.0f;
	float frontTargetRot = 15.0f;

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
