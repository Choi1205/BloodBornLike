// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Practice/RandomMoveEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy/Practice/RandomMoveEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values
ARandomMoveEnemy::ARandomMoveEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));//���Ǿ� �ݸ��� ������Ʈ�� �����Ͽ� PlayerCollisionDetection�̸����� �����Ѵ�

	PlayerCollisionDetection->SetupAttachment(RootComponent);//������ ���Ǿ� �ݸ��� ������Ʈ�� ��Ʈ�� ���δ�.

	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));//���Ǿ� �ݸ��� ������Ʈ�� �����Ͽ� PlayerAttackCollisionDetection�̸����� �����Ѵ�

	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);//������ ���Ǿ� �ݸ��� ������Ʈ�� ��Ʈ�� ���δ�.

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//�ڽ� �ݸ��� ������Ʈ�� �����Ͽ� DamageCollision�̸����� �����Ѵ�.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//������ �ڽ� �ݸ��� ������Ʈ�� �Ž��� ���δ�.

}

// Called when the game starts or when spawned
void ARandomMoveEnemy::BeginPlay()
{
	Super::BeginPlay();//��ӹ��� BeginPlay�� �״�� ���. �Ʒ��� ������ �߰���.
	
	RandomMoveEnemyAIController = Cast<ARandomMoveEnemyAIController>(GetController());//AI��Ʈ�ѷ��� �����Ͽ� ����ϱ� ���� ���� �ʱ�ȭ
	RandomMoveEnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ARandomMoveEnemy::OnAIMoveCompleted);//��Ʈ�ѷ��� �ൿ�� ����� ���� �޾ƿ�(?)

	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ARandomMoveEnemy::OnPlayerDetectedOverlapBegin);
	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ARandomMoveEnemy::OnPlayerDetectedOverlapEnd);
	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ARandomMoveEnemy::OnPlayerAttackOverlapBegin);
	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ARandomMoveEnemy::OnPlayerAttackOverlapEnd);
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ARandomMoveEnemy::OnDealDamageOverlapBegin);
	//�� ������ �̹�Ʈ���� �߻��ϸ� ()���� �Լ��� ����ǵ��� ���ε� ��.

	AnimInstance = GetMesh()->GetAnimInstance();
	//�ִϸ��̼� �ν��Ͻ��� �ʱ�ȭ
}

// Called every frame
void ARandomMoveEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARandomMoveEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARandomMoveEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {//�̵��� ����Ǿ�����
	if (!PlayerDetected) {//�÷��̾ ���������� ������
		RandomMoveEnemyAIController->RandomPatrol();//����Ž���� �Ѵ�
	}
	else if (PlayerDetected && CanAttackPlayer) {//�÷��̾ ���������� �ְ�, ���ݹ��� �ȿ��� ������
		StopSeekingPlayer();//������ �����ϰ�

		AnimInstance->Montage_Play(EnemyAttackAnimation);//�÷��̾�� ���� �ൿ�� �Ѵ�
	}
}

void ARandomMoveEnemy::MoveToPlayer() {//�÷��̾�� �̵�
	RandomMoveEnemyAIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);//�����̵� ������ �÷��̾� ������ �Է��ؼ� �����̵��� �÷��̾� �̵��� �ǰ� �Ѵ�.
}

void ARandomMoveEnemy::SeekPlayer() {//�÷��̾��� ��ġ�� ����
	MoveToPlayer();//�÷��̾�� �̵��Ѵ�
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this, &ARandomMoveEnemy::SeekPlayer, 0.25f, true);//�÷��̾��� ��ġ�� 0.25�ʸ��� �����Ѵ�.
}

void ARandomMoveEnemy::StopSeekingPlayer() {//�÷��̾� ���� �ߴ�
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);//�÷��̾� ��ġ���� Ÿ�̸Ӹ� �����ؼ� ������ �����.
}

void ARandomMoveEnemy::OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//�÷��̾ Ž�������� ���� ���
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		PlayerDetected = true;//�÷��̾� Ž���� true��
		SeekPlayer();//���� ����
	}
}

void ARandomMoveEnemy::OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	//�÷��̾ Ž�������� �������� ���
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//�������� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		PlayerDetected = false;//�÷��̾� Ž���� false��
		StopSeekingPlayer();//���� ����
		RandomMoveEnemyAIController->RandomPatrol();//���� Ž���� ����
	}
}

void ARandomMoveEnemy::OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//�÷��̾ ���ݰ��� ������ ���� ���
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		CanAttackPlayer = true;//���� ���� ������ true��
	}
}

void ARandomMoveEnemy::OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//�������� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF) {//�÷��̾��
		CanAttackPlayer = false;//���� ���� ������ false��

		AnimInstance->Montage_Stop(0.0f, EnemyAttackAnimation);
		//������̴� ���� �ִϸ��̼��� ���(0.0f) ������
		SeekPlayer();//�׸��� ������ ������
	}
}

void ARandomMoveEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (PlayerREF && CanDealDamage) {//�÷��̾��̰�, ���ݵ� �����ϴٸ�
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged"));//�÷��̾�� �������� �Ծ���.
	}
}

void ARandomMoveEnemy::AttackAnimationEnded()
{
	if (CanAttackPlayer) {//�÷��̾ ���� ���� ������ ���� ���
		AnimInstance->Montage_Play(EnemyAttackAnimation);
		//���� �ִϸ��̼��� �ٽ� ����Ѵ�.
	}
}