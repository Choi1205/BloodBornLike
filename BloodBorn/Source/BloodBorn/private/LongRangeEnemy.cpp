// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/SceneComponent.h"
#include "BulletActor.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));
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

	//�÷��̾ �������� ���
	if (CanSeePlayer) {
		//�÷��̾� ��ħ Ÿ�̸Ӱ� ���ư���.
		lostPlayerTimer += DeltaTime;
		//�÷��̾ ��ġ�� 5�ʰ� ������ 
		if (lostPlayerTimer > 5.0f) {
			//Ÿ�̸Ӹ� �����ϰ�
			lostPlayerTimer = 0.0f;
			//���� Ÿ�̸ӵ� �����ϰ�
			AimmingTime = 0.0f;
			//�÷��̾� ������ �����Ѵ�.
			SetCanSeePlayer(false, nullptr);
		}
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
	}
}

void ALongRangeEnemy::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	//�þ߿� �÷��̾ ��������
	if (SeePlayer) {
		//�þ߿� �÷��̾ �ִµ��� Ÿ�̸� ����
		lostPlayerTimer = 0.0f;
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
		//�÷��̾� ���� Ʈ���Ÿ� ����
		CanSeePlayer = SeePlayer;
	}
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
	//�и��Ǵ� ������ �����Ƿ� ������ ó���� ȣ��
	GotDamagedCPP(Damage);
}

void ALongRangeEnemy::GetHit(const FVector& ImpactPoint)
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

float ALongRangeEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

