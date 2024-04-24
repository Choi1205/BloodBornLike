// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GamePlayStatics.h"
#include "NiagaraComponent.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h"
#include "HUD/BBPlayerHUD.h"
#include "HUD/WeaponOverlay.h"
#include "GameFramework/Controller.h"




AWeapon::AWeapon()
{
	SawBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Saw Box"));
	SawBox->SetupAttachment(GetRootComponent());
	SawBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SawBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SawBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

	sawEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Saw Effect"));
	sawEffect->SetupAttachment(GetRootComponent());
	sawEffect->bAutoActivate = /*true*/false;

// 	equipSawOverlay = CreateDefaultSubobject<UWidgetComponent>(TEXT("EquipSaw Widget"));
// 	equipSawOverlay->SetupAttachment(GetRootComponent());
// 	equipSawOverlay->SetWidgetSpace(EWidgetSpace::Screen);
// 	equipSawOverlay->SetVisibility(false);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	SawBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
	sawEffect->Activate(true);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	/*sawEffect->Activate(false);*/
	//equipSawOverlay->/*SetVisibility(false)*/DestroyComponent(true);
	if (weaponOverlay)
	{
	weaponOverlay->img_Saw->SetVisibility(ESlateVisibility::Hidden);
	}

	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	player = Cast<ABloodBornCharacter>(NewOwner);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	/*equipSawOverlay->SetVisibility(true);*/
	if (weaponOverlay)
	{
	weaponOverlay->img_Saw->SetVisibility(ESlateVisibility::Visible);
	}
// 	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
// 	{
// 		ABBPlayerHUD* BBPlayerHUD = Cast<ABBPlayerHUD>(PlayerController->GetHUD());
// 		if (BBPlayerHUD)
// 		{
// 			BBPlayerHUD->showWeaponOverlay();
// 		}
// 	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//equipSawOverlay->SetVisibility(false);
	if (weaponOverlay)
	{
	weaponOverlay->img_Saw->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();  // ��������̼�
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle( 
			this, Start, End,
			FVector(5.f, 5.f, 5.f), BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, BoxHit, true
	);
	
	//if (BoxHit.GetActor())
	//{

	//	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());  // enemy�� ���� IHitInterface ��ü�� ĳ��Ʈ ����
	//	
	//	if (HitInterface)
	//	{
	//		HitInterface->GetHit(BoxHit.ImpactPoint);
	//	}
	//	IgnoreActors.AddUnique(BoxHit.GetActor());

	//	UGameplayStatics::ApplyDamage(
	//			BoxHit.GetActor(),
	//			Damage,
	//			GetInstigator()->GetController(),
	//			this,
	//			UDamageType::StaticClass()
	//	);
	//}
	if (BoxHit.GetActor())
	{

		/*
		if (player->attackType == 0)
		{
			ApplyDamage(BoxHit.GetActor(), Damage, EAttackType::Normal);
		}
		else if (player->attackType == 1)
		{
			ApplyDamage(BoxHit.GetActor(), HeavyDamage, EAttackType::Normal);
		}
		*/
		float DamagedApplied = ApplyDamage(BoxHit.GetActor(), Damage, EAttackType::Normal);
		if (DamagedApplied > 0.f)
		{
			IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
			if (HitInterface)
			{
				HitInterface->GetHit(BoxHit.ImpactPoint);
				UE_LOG(LogTemp, Warning, TEXT("Normal Attack"));

			}

			// ������ ����� ��쿡�� ȣ��
			// OnSuccessfulAttack(DamagedApplied);
			// OnSuccessfulAttackEvent.Broadcast(DamagedApplied);
		}
	}
	else if (BoxHit.GetActor())
	{
		float DamagedApplied = ApplyDamage(BoxHit.GetActor(), Damage, EAttackType::Heavy);
		if (DamagedApplied > 0.f)
		{
			IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
			if (HitInterface)
			{
				HitInterface->GetHit(BoxHit.ImpactPoint);
			}

			// ������ ����� ��쿡�� ȣ��
			// OnSuccessfulAttack(DamagedApplied);
			// OnSuccessfulAttackEvent.Broadcast(DamagedApplied);
		}
	}
}



//float AWeapon::ApplyDamage(AActor* DamagedActor)
//{
//	if (!DamagedActor) return 0.f;
//	
//	float ActualDamage = UGameplayStatics::ApplyDamage(
//																			DamagedActor,
//																			Damage,
//																			GetInstigator()->GetController(),
//																			this,
//																			UDamageType::StaticClass()
//																		);
//	return ActualDamage;
//}
float AWeapon::ApplyDamage(AActor* DamagedActor, float DamageAmount, EAttackType AttackType)
{
	if (!DamagedActor) return 0.f;
	UE_LOG(LogTemp, Warning, TEXT("DamageAmount : %f"), DamageAmount);
	if (player != nullptr)
	{
		if (player->attackType == 0)
		{
			player->OnSuccessfulAttack(DamageAmount, EAttackType::Normal);
		}
		else if (player->attackType == 1)
		{
			player->OnSuccessfulAttack(DamageAmount, EAttackType::Heavy);
		}
		else {
			//����̱� ȸ��
			//ItemMesh->SetVisibility(false);
			player->OnSuccessfulAttack(DamageAmount, EAttackType::HoldAttack);
		}
	}
	float ActualDamage = UGameplayStatics::ApplyDamage(
		DamagedActor,
		DamageAmount,
		GetInstigator()->GetController(),
		this,
		UDamageType::StaticClass()
	);
	 return ActualDamage;


	OnSuccessfulAttackEvent.Broadcast(DamageAmount, AttackType);

	//if (!DamagedActor || !DamagedActor->GetClass()->ImplementsInterface(UHitInterface::StaticClass()))
	//{
	//	// DamagedActor�� ��ȿ�� ����� �ƴϰų� HitInterface�� �������� ���� ��� ���ظ� �� ����
	//	return 0.f;
	//}
	//// DamagedActor�� HitInterface�� �����ߴ��� Ȯ��
	////if (DamagedActor->GetClass()->ImplementsInterface(UHitInterface::StaticClass()))
	////{
	//	// ���ظ� ������ ������ ���� ���ط� �޾ƿ�
	//	float ActualDamage = UGameplayStatics::ApplyDamage(
	//		DamagedActor,
	//		DamageAmount,
	//		GetInstigator()->GetController(),
	//		this,
	//		UDamageType::StaticClass()
	//	);

	//	// ���ظ� ���� ��쿡�� �̺�Ʈ ��ε�ĳ��Ʈ
	//	if (ActualDamage > 0.f)
	//	{
	//		OnSuccessfulAttackEvent.Broadcast(DamageAmount, AttackType);
	//	}

	//	return ActualDamage;
	//}
}
//void AWeapon::ApplyDamage(AActor* DamagedActor, float DamageAmount, EAttackType AttackType)
//{
//	// if (!DamagedActor) return 0.f;
//
//	float ActualDamage = UGameplayStatics::ApplyDamage(
//		DamagedActor,
//		DamageAmount,
//		GetInstigator()->GetController(),
//		this,
//		UDamageType::StaticClass()
//	);
//	// return ActualDamage;
//
//
//	OnSuccessfulAttackEvent.Broadcast(DamageAmount, AttackType);
//
//}