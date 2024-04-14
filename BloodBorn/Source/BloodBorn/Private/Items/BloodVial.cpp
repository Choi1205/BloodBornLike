// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BloodVial.h"
#include "Interfaces/PickupInterface.h"
#include "BloodBorn/BloodBornCharacter.h"

void ABloodVial::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		// PickupInterface->AddBloodVials(this);  // 지금은 바로 오버랩되면 얻을 수 있는데 FkeyPressed로 바꿔야함 
		PickupInterface->SetOverlappingItem(this);
	}

	// Destroy();  // 이것도 destroy인데 인게임에서는 서서히 사라짐 흠 ㅋ
}

void ABloodVial::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void ABloodVial::Equip(/*USceneComponent* InParent, *//*AActor* NewOwner, APawn* NewInstigator)*/ AActor* OtherActor)
{
// 	SetOwner(NewOwner);
// 	SetInstigator(NewInstigator);
// 	// player = Cast<ABloodBornCharacter>(NewOwner);
// 
// 	Destroy();

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		 PickupInterface->AddBloodVials(this);

		 Destroy();
	}
}

//void ABloodVial::GetBVs()
//{
//	Destroy();
//}

//void ABloodVial::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
//{
//	SetOwner(NewOwner);
//	SetInstigator(NewInstigator);
//	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
//	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
//	player = Cast<ABloodBornCharacter>(NewOwner);
//}



//void ABloodVial::AddBloodVials()
//{ GetBloodVials 해야..하나? 
// 
// Destroy();
//}
