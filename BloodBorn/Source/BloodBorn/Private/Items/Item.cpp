#include "Items/Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
// #include "BloodBorn/BloodBornCharacter.h"
#include "Interfaces/PickupInterface.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh Component"));
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//Sphere->SetupAttachment(ItemMesh);
	Sphere->SetupAttachment(GetRootComponent());

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ABloodBornCharacter* BBCharacter = Cast<ABloodBornCharacter>(OtherActor);
	//if (BBCharacter)
	//{
	//	BBCharacter->SetOverlappingItem(this);
	//}
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//ABloodBornCharacter* BBCharacter = Cast<ABloodBornCharacter>(OtherActor);
	//if (BBCharacter)
	//{
	//	BBCharacter->SetOverlappingItem(nullptr);
	//}
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

