// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RandomMoveEnemy.generated.h"

UCLASS()
class BLOODBORN_API ARandomMoveEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARandomMoveEnemy();

	bool PlayerDetected;//�÷��̾� �߽߰� true
	bool CanAttackPlayer;//�÷��̾ �����ൿ ������ ���� �� true

	UPROPERTY(BlueprintReadWrite)
		bool CanDealDamage;//��������Ʈ������ ���� ����. �÷��̾ ���� ������ ���� �� true

	class ABloodBornCharacter* PlayerREF;//�÷��̾� ĳ���͸� ����. Ŭ�������� A+������Ʈ��+Charactor
	
	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerCollisionDetection;//��𼭵� ���� ����. �÷��̾� Ž������

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerAttackCollisionDetection;//�����ൿ ����

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;//���ݹ���

	class ARandomMoveEnemyAIController* RandomMoveEnemyAIController;//AI��Ʈ�ѷ��� ����� ����ϱ� ���� ��Ʈ�ѷ��� ���� ������ �ʿ��ϴ�.
	void OnAIMoveCompleted(struct FAIRequestID RequsetID, const struct FPathFollowingResult& Result);//AI�� �̵��� �������� �˷���.

	UPROPERTY(EditAnywhere)
		float StoppingDistance = 100.0f;//�÷��̾�� 100.0f��ġ�� �����ϸ� �����. �Ž� �浹������ 0�Ÿ��� �������ϸ� �̻�������.

		FTimerHandle SeekPlayerTimerHandle;//�÷��̾� Ž���� �����ð����� �������� Ž�� ������ �����µ�, �� �ð��� ��Ʈ���ϴ� �ڵ鷯 ����

	UFUNCTION()
		void MoveToPlayer();//�÷��̾�� �̵�

	UFUNCTION()
		void SeekPlayer();//�÷��̾� ��ġ�� ���������� ����

	UFUNCTION()
		void StopSeekingPlayer();//�÷��̾� ��ġ ������ �ߴ�, �����̵����� ��ȯ

	UFUNCTION()
		void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		//�÷��̾ Ž�������� ��������. �÷��̾� ������ ������
	UFUNCTION()
		void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		//�÷��̾ Ž�������� ������������. �����̵����� ��ȯ��
	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		//�÷��̾ ���� ���ɹ����� ������ ��. �̵��� ���߰� ���� �ൿ���� ��ȯ��
	UFUNCTION()
		void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		//�÷��̾ ���� ���� ������ ����� ��. �÷��̾� ������ ������.
	UFUNCTION()
		void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		//�÷��̾ ���� ���� ������ �ְ�, ���� �ൿ�� �浹������ �¾�����. �������� �Ծ�� �ϳ�, ������ �޼����� �����.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};