// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LadyMariaAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ALadyMariaAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	class ABloodBornCharacter* playerREF;

	ALadyMariaAIController();

	void BeginPlay() override;

	class ABloodBornCharacter* FindPlayer_BP();
};
