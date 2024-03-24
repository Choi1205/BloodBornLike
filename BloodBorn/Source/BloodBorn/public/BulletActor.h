// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class BLOODBORN_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

	UPROPERTY(EditAnywhere)
	class USphereComponent* bulletBody;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* bulletShape;

	UPROPERTY(EditAnywhere)
	float bulletSpeed = 1000.0f;

	class ABloodBornCharacter* PlayerREF;

	UFUNCTION()
	void CallHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float lifeTime = 5.0f;
	float lifeTimeStack = 0.0f;

	float damage = 20.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
