// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPTreeEnemy.h"
#include "BloodBornCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
//#include "BTAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

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

}

// Called when the game starts or when spawned
void ACPPTreeEnemy::BeginPlay()
{
	Super::BeginPlay();
	/*
	BTAIController = Cast<ABTAIController>(GetController());//AI��Ʈ�ѷ��� �����Ͽ� ����ϱ� ���� ���� �ʱ�ȭ
	
	BTAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ACPPTreeEnemy::OnAIMoveCompleted);//��Ʈ�ѷ��� �ൿ�� ����� ���� �޾ƿ�(?)
	
	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapBegin);
	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapEnd);
	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapBegin);
	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapEnd);
	*/
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnDealDamageOverlapBegin);
	
	//AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACPPTreeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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