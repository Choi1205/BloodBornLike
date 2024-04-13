// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CPPTreeStayEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemy/BTStayAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACPPTreeStayEnemy::ACPPTreeStayEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	weaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//�ڽ� �ݸ��� ������Ʈ�� �����Ͽ� DamageCollision�̸����� �����Ѵ�.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//������ �ڽ� �ݸ��� ������Ʈ�� �Ž��� ���δ�.

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

// Called when the game starts or when spawned
void ACPPTreeStayEnemy::BeginPlay()
{
	Super::BeginPlay();

	BTStayAIController = Cast<ABTStayAIController>(GetController());//AI��Ʈ�ѷ��� �����Ͽ� ����ϱ� ���� ���� �ʱ�ȭ

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeStayEnemy::OnDealDamageOverlapBegin);

	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACPPTreeStayEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BTStayAIController != nullptr) {
		if (BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			stunTimer += DeltaTime;
		}

		if (stunTimer > 3.0f) {
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), false);
			stunTimer = 0.0f;
		}

		if (BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == true) {
			AfterAttackMoving(DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ACPPTreeStayEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPPTreeStayEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF && CanDealDamage) {//�÷��̾��̰�, ���ݵ� �����ϴٸ�
		//�÷��̾�� �������� �Ծ���.
		UGameplayStatics::ApplyDamage(PlayerREF, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
}

void ACPPTreeStayEnemy::GetHit(const FVector& ImpactPoint)
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

float ACPPTreeStayEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

	GotDamage(DamageAmount);

	return DamageAmount;
}

void ACPPTreeStayEnemy::GotParryAttackCPP(float damage)
{
	GotDamage(damage);

	if (BTStayAIController != nullptr) {
		if (CanParryed) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			CanParryed = false;
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), true);
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
		}
	}
	//�и����� �� ���������� ��
	UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
}

bool ACPPTreeStayEnemy::GetInStun()
{
	return BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"));
}

float ACPPTreeStayEnemy::GetHealth()
{
	return healthPoint;
}

void ACPPTreeStayEnemy::AfterAttackMoving(float DeltaTime)
{
	if (healthPoint > 0) {
		FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector towardPlayer = playerLocation - GetActorLocation();

		FVector randomLocation = BTStayAIController->GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));
		FVector nextMovePoint = randomLocation - GetActorLocation();

		float randomTime = BTStayAIController->GetBlackboardComponent()->GetValueAsFloat(FName("RandomTime"));

		UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
		AddMovementInput(nextMovePoint.GetSafeNormal(1.0));
		SetActorRotation(towardPlayer.Rotation());

		randomTime -= DeltaTime;

		BTStayAIController->GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);
	}
}

void ACPPTreeStayEnemy::GotDamage(float damage)
{
	healthPoint -= damage;

	//���� ����Ʈ �����. �������Ʈ�� NiaSys�� �̸� ��ƼŬ�� ����ؾ��Ѵ�.
	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator);

	if (BTStayAIController != nullptr) {
		if (healthPoint <= 0) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			BTStayAIController->PawnSensing->Deactivate();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AnimInstance->Montage_Play(EnemyDyingAnimation);
			BTStayAIController->UnPossess();
			BTStayAIController = nullptr;
		}
		else {
			if (!BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), true);
				AnimInstance->Montage_Play(EnemyHitAnimation);
			}
		}
	}
}

void ACPPTreeStayEnemy::DyingAnimEnd()
{
	AnimInstance->Montage_Pause();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetSimulatePhysics(true);
}

void ACPPTreeStayEnemy::HitAnimEnd()
{
	if (healthPoint > 0) {
		BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
	}
}
