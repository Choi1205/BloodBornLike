// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BloodBornCharacter.h"
#include "Components/SceneComponent.h"
#include "BulletActor.h"

// Sets default values
ALongRangeEnemy::ALongRangeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�� ���� ������Ʈ�� ����, ����
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	//źȯ �߻� ����Ʈ�� ����, ����.
	//SetupAttachment�� ���ڰ����� RootComponent�� �ָ� C++���� ��Ʈ ������Ʈ�� �������� �ʴ���, �ڵ����� ���ϵ尡 ��.
	bulletFirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletFirePoint"));
	bulletFirePoint->SetupAttachment(RootComponent);
	bulletFirePoint->SetRelativeLocation(FVector(60.0f, 10.0f, 50.0f));

}

// Called when the game starts or when spawned
void ALongRangeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//�þ߿� ���� ���� ������ OnSeePawn�� �۵��ϵ��� ���ε�
	PawnSensing->OnSeePawn.AddDynamic(this, &ALongRangeEnemy::OnSeePawn);

}

// Called every frame
void ALongRangeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//�÷��̾ ���� ���ؽð��� ���
	if (CanSeePlayer) {
		AimmingTime += DeltaTime;
	}

	//�ִԺ������Ʈ���� SetFireGun�� �ʱ�ȭ�� ����� ã�� �� ����, ������ ������ �ʱ�ȭ �ǽ�.
	if (SetFireGun && AimmingTime > 1.0f) {
		SetFireGun = false;
	}

	//3�� �� źȯ�� �߻��Ѵ�.
	if (AimmingTime > 3.0f) {
		SetFireGun = true;
		FireBullet();
		AimmingTime = 0.0f;
	}
}

// Called to bind functionality to input
void ALongRangeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALongRangeEnemy::OnSeePawn(APawn* PlayerPawn)
{
	//�þ߿� �÷��̾ ���ö��� Player�� Nullptr�� �ƴϴ�
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(PlayerPawn);

	//�þ߿� �÷��̾ ������
	if (Player) {
		//SetCanSeePlayer�� ����
		SetCanSeePlayer(true, Player);
		//RunRetriggerableTimer();
		//������ ����Ż. �ȳ����� ������ ����������� ���� ��� ���
		/*����Ż ����
		Fatal error: [File:D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\UObject\ScriptCore.cpp] [Line: 1459] Failed to find function SetCanSeePlayer in BP_LongRangeEnemy_C /Game/Choi/Level/UEDPIE_0_EnemyLevel.EnemyLevel:PersistentLevel.BP_LongRangeEnemy_C_1

		UnrealEditor_CoreUObject
		UnrealEditor_BloodBorn_0111!ALongRangeEnemy::RunRetriggerableTimer() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Source\BloodBorn\private\LongRangeEnemy.cpp:75]

		UnrealEditor_BloodBorn_0111!ALongRangeEnemy::OnSeePawn() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Source\BloodBorn\private\LongRangeEnemy.cpp:58]

		UnrealEditor_BloodBorn_0111!ALongRangeEnemy::execOnSeePawn() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Intermediate\Build\Win64\UnrealEditor\Inc\BloodBorn\UHT\LongRangeEnemy.gen.cpp:26]
		�������� �̷�� �� �� ��������Ʈ�� ������ �Լ����� ã�� ���ϴ� ������ ����. �׷��� �Լ����� ��ġ��. �����Ͽ� Ȯ�� �ʿ�.
		*/
	}
}

void ALongRangeEnemy::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	//�þ߿� �÷��̾ ��������
	if (SeePlayer) {
		//CanSeePlayer�� ������Ʈ �ӽ��� ���� �ִϸ��̼� �ߵ� Ʈ����
		CanSeePlayer = SeePlayer;
		//�÷��̾�� ĳ����
		PlayerREF = Cast<ABloodBornCharacter>(Player);
		//�÷��̾ ���� ���͸� ���. ������ - �����
		FVector towardPlayer= PlayerREF->GetActorLocation() - GetActorLocation();
		//�÷��̾ �ٶ󺸰� �ؼ� �����ϴ� �� ó�� ���̰� ��. Pitch������ ���Ʒ� �����̹Ƿ� ������ ���� Pitch�� ����
		SetActorRotation(FRotator(GetActorRotation().Pitch, towardPlayer.Rotation().Yaw, towardPlayer.Rotation().Roll));
	}
	else {
		//���� ���۵�. RunRetriggerableTimer()�� ���ľ���
		UE_LOG(LogTemp, Warning, TEXT("Exit aim?"));
		CanSeePlayer = SeePlayer;
	}
}

void ALongRangeEnemy::RunRetriggerableTimer()
{
	//�þ߿��� ��� �� �÷��̾ �����ϴ� Ÿ�̸Ӹ� ����
	//�÷��̾ �þ߿� ������ ��� �� �Լ��� ȣ��Ǽ� Ÿ�̸Ӱ� ��� ���µȴ�.
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	//Ÿ�̸� ���� �� SetCanSeePlayer()�� �����ϵ��� ���ε�
	FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, nullptr);

	//������ �ð�(PawnSensing->SensingInterval * TrackingPlayer)�� ����ϸ� ���ε��� �Լ��� �����
	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
}

void ALongRangeEnemy::FireBullet()
{
	//źȯ�� �߻���ġ �浹������ ����
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//źȯ�� �߻��Ѵ�
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), GetActorRotation(), params);
}

void ALongRangeEnemy::GotDamagedCPP(float Damage)
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

void ALongRangeEnemy::GotParryAttackCPP(float Damage)
{
	GotDamagedCPP(Damage);
}

