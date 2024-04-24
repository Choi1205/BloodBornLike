// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShortCutActor.generated.h"

UCLASS()
class BLOODBORN_API AShortCutActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShortCutActor();

	UPROPERTY(EditAnywhere)
	class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* moveStartArea;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* platform;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MovePlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	class UBloodBornGameInstance* gi;

	UPROPERTY(EditAnywhere)
	FVector bottomPlace = FVector(-2939.0f, 4210.0f, -23.0f);
	UPROPERTY(EditAnywhere)
	FVector topPlace = FVector(-2939.0f, 4210.0f, 1472.0f);

	FVector startLoc;
	FVector targetLoc;

	float movingTime = 5.0f;
	float currentTime = 0.0f;
};
