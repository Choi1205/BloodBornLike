


#include "Enemy/Enemy76.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AttributeComponent.h"


AEnemy76::AEnemy76()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);  // (saw box������) visibility channel�� block�ؾ���
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); // ī�޶� collide���� �ʵ��� (����� �� ���εǴ� �� ����)
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

}

void AEnemy76::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEnemy76::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemy76::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy76::GetHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint"));

	// ���� �� Dot ���� ������ �� �ִϸ��̼ǵ� ���� ���߿� �ʿ����� �ƴ����� �𸣰���
	// �� ������ �ִϸ��̼��� ��� Theta ������ �� 0�ΰǰ� 
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation().GetSafeNormal());

	// Forward * ToHit = | Forward | | ToHit | * cos(theta), | Forward | | ToHit | : normalized����
	// | Forward | = 1,  | ToHit | = 1, �׷��Ƿ� Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);  //Forward ��ֶ������ؼ� ���� 1�̶� 60 ����
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);

}

float AEnemy76::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("Damage"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, FString::Printf(TEXT("%f"), DamageAmount));
		}
		// ���⿡ ü�¹�(?)
	}
	return DamageAmount;
}

void AEnemy76::GotParryAttackCPP(float damage)
{
}

