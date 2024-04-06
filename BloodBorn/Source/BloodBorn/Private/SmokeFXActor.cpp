// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeFXActor.h"
#include "NiagaraComponent.h"

// Sets default values
ASmokeFXActor::ASmokeFXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(rootComp);

	particleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SmokeVFX1"));
	particleComp->SetupAttachment(rootComp);
	particleComp->SetRelativeLocation(FVector(0, 0, 90));
	particleComp->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ASmokeFXActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmokeFXActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmokeFXActor::PlayFX()
{
	particleComp->Activate(true);
}
