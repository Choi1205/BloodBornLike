// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPTreeEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "BTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

/*
�÷��̾ �ν��� ���¿��� ������ �߻��ϴ� ����.
�ױ����� �� ������ ���ְ�, �� ���� �κп� �����ִ��� üũ�ϴ� if���� ������ �����ϴ�?
Unhandled Exception: EXCEPTION_ACCESS_VIOLATION reading address 0x00000000000004a8

UnrealEditor_BloodBorn_0046!UE::CoreUObject::Private::ResolveObjectHandle() [C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\ObjectHandle.h:217]
UnrealEditor_BloodBorn_0046!ABTAIController::SetCanSeePlayer() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Source\BloodBorn\Private\BTAIController.cpp:59]
UnrealEditor_BloodBorn_0046!ABTAIController::execSetCanSeePlayer() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Intermediate\Build\Win64\UnrealEditor\Inc\BloodBorn\UHT\BTAIController.gen.cpp:36]
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_Engine
UnrealEditor_BloodBorn_0046!TBaseUFunctionDelegateInstance<ABTAIController,void __cdecl(void),FNotThreadSafeNotCheckedDelegateUserPolicy,bool,APawn *>::Execute() [C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Core\Public\Delegates\DelegateInstancesImpl.h:154]
*/

// Sets default values
ACPPTreeEnemy::ACPPTreeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*
	PlayerCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));//���Ǿ� �ݸ��� ������Ʈ�� �����Ͽ� PlayerCollisionDetection�̸����� �����Ѵ�
	PlayerCollisionDetection->SetupAttachment(RootComponent);//������ ���Ǿ� �ݸ��� ������Ʈ�� ��Ʈ�� ���δ�.
	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));//���Ǿ� �ݸ��� ������Ʈ�� �����Ͽ� PlayerAttackCollisionDetection�̸����� �����Ѵ�
	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);//������ ���Ǿ� �ݸ��� ������Ʈ�� ��Ʈ�� ���δ�.
	*/
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//�ڽ� �ݸ��� ������Ʈ�� �����Ͽ� DamageCollision�̸����� �����Ѵ�.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//������ �ڽ� �ݸ��� ������Ʈ�� �Ž��� ���δ�.

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ACPPTreeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	BTAIController = Cast<ABTAIController>(GetController());//AI��Ʈ�ѷ��� �����Ͽ� ����ϱ� ���� ���� �ʱ�ȭ
	/*
	BTAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ACPPTreeEnemy::OnAIMoveCompleted);//��Ʈ�ѷ��� �ൿ�� ����� ���� �޾ƿ�(?)
	
	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapBegin);
	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapEnd);
	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapBegin);
	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapEnd);
	*/
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnDealDamageOverlapBegin);
	
	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACPPTreeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BTAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		stunTimer += DeltaTime;
	}

	if (stunTimer > 3.0f) {
		BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), false);
		stunTimer = 0.0f;
	}
}

// Called to bind functionality to input
void ACPPTreeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/*
void ACPPTreeEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {//�̵��� ����Ǿ�����
	if (!PlayerDetected) {//�÷��̾ ���������� ������
		BTAIController->RandomPatrol();//����Ž���� �Ѵ�
	}
	else if (PlayerDetected && CanAttackPlayer) {//�÷��̾ ���������� �ְ�, ���ݹ��� �ȿ��� ������
		StopSeekingPlayer();//������ �����ϰ�

		AnimInstance->Montage_Play(EnemyAttackAnimation);//�÷��̾�� ���� �ൿ�� �Ѵ�
	}
}

void ACPPTreeEnemy::MoveToPlayer() {//�÷��̾�� �̵�
	BTAIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);//�����̵� ������ �÷��̾� ������ �Է��ؼ� �����̵��� �÷��̾� �̵��� �ǰ� �Ѵ�.
}

void ACPPTreeEnemy::SeekPlayer() {//�÷��̾��� ��ġ�� ����
	MoveToPlayer();//�÷��̾�� �̵��Ѵ�
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this, &ACPPTreeEnemy::SeekPlayer, 0.25f, true);//�÷��̾��� ��ġ�� 0.25�ʸ��� �����Ѵ�.
}

void ACPPTreeEnemy::StopSeekingPlayer() {//�÷��̾� ���� �ߴ�
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);//�÷��̾� ��ġ���� Ÿ�̸Ӹ� �����ؼ� ������ �����.
}

void ACPPTreeEnemy::OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//�÷��̾ Ž�������� ���� ���
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		PlayerDetected = true;//�÷��̾� Ž���� true��
		SeekPlayer();//���� ����
	}
}

void ACPPTreeEnemy::OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	//�÷��̾ Ž�������� �������� ���
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//�������� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		PlayerDetected = false;//�÷��̾� Ž���� false��
		StopSeekingPlayer();//���� ����
		BTAIController->RandomPatrol();//���� Ž���� ����
	}
}

void ACPPTreeEnemy::OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//�÷��̾ ���ݰ��� ������ ���� ���
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		CanAttackPlayer = true;//���� ���� ������ true��
	}
}

void ACPPTreeEnemy::OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//�������� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		CanAttackPlayer = false;//���� ���� ������ false��

		AnimInstance->Montage_Stop(0.0f, EnemyAttackAnimation);
		//������̴� ���� �ִϸ��̼��� ���(0.0f) ������
		SeekPlayer();//�׸��� ������ ������
	}
}
*/
void ACPPTreeEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF && CanDealDamage) {//�÷��̾��̰�, ���ݵ� �����ϴٸ�
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged"));//�÷��̾�� �������� �Ծ���.
	}
}
/*
void ACPPTreeEnemy::AttackAnimationEnded()
{
	if (CanAttackPlayer) {//�÷��̾ ���� ���� ������ ���� ���
		AnimInstance->Montage_Play(EnemyAttackAnimation);
		//���� �ִϸ��̼��� �ٽ� ����Ѵ�.
	}
}*/

void ACPPTreeEnemy::GotDamagedCPP(float Damage)
{
	//�÷��̾� HP UI ������ -HP����
	healthPoint -= Damage;
	if (healthPoint < 0) {
		healthPoint = 0;
	}

	//�������� �޾� ������ �����Ѵ�.
	//����ġ ó���� ����
	UE_LOG(LogTemp, Warning, TEXT("Remain HP : %.0f"), healthPoint);

	if (healthPoint == 0) {
		//���ó���κ�. �ִϸ��̼� ��� �ʿ�
		//Destroy();
	}
}

void ACPPTreeEnemy::GotParryAttackCPP(float Damage)
{
	//�ϴ� �������� ��...������ õõ�� �����ϱ�
	GotDamagedCPP(Damage);
	
	if (CanParryed) {
		if (AnimInstance->Montage_IsPlaying(NULL)) {
			AnimInstance->Montage_Stop(NULL);
		}
		CanParryed = false;
		BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), true);
		
	}
	//�и����� �� ���������� ��
	UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), Damage);
}

void ACPPTreeEnemy::GetHit(const FVector& ImpactPoint)
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

float ACPPTreeEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), DamageAmount);
	healthPoint -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Remain HP : %f"), healthPoint);
	UE_LOG(LogTemp, Warning, TEXT("Health P : %f"), Attributes->GetHealthPercent());

	if (healthPoint <= 0 && !bIsDead) {
		if (AnimInstance->Montage_IsPlaying(NULL)) {
			AnimInstance->Montage_Stop(NULL);
			bIsDead = true;
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AnimInstance->Montage_Play(EnemyDyingAnimation);
		BTAIController->UnPossess();
	}
	
	return DamageAmount;
}

void ACPPTreeEnemy::DyingAnimEnd()
{
	AnimInstance->Montage_Pause();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}