// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "BloodDecalActor.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ABloodDecalActor : public ADecalActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TArray<class UMaterialInterface*> bloodArray;

	void ForAttachPlayer();
};
