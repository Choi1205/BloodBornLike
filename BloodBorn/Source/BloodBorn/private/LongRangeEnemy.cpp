// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "BulletActor.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/MovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ALongRangeEnemy::ALongRangeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//폰 감지 컴포넌트를 생성, 부착
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	gunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	gunMesh->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	//탄환 발사 포인트를 생성, 부착.
	//SetupAttachment에 인자값으로 RootComponent를 주면 C++에서 루트 컴포넌트를 지정하지 않더라도, 자동으로 차일드가 됨.
	bulletFirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletFirePoint"));
	bulletFirePoint->SetupAttachment(RootComponent);
	bulletFirePoint->SetRelativeLocation(FVector(60.0f, 10.0f, 50.0f));

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void ALongRangeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//시야에 들어온 폰이 있으면 OnSeePawn이 작동하도록 바인딩
	PawnSensing->OnSeePawn.AddDynamic(this, &ALongRangeEnemy::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &ALongRangeEnemy::OnHearNoise);
}

// Called every frame
void ALongRangeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (health > 0) {
		//플레이어를 보면 조준시간을 계산
		if (CanSeePlayer) {
			AimmingTime += DeltaTime;
		}

		//애님블루프린트에서 SetFireGun을 초기화할 방법을 찾을 수 없어, 적당한 시점에 초기화 실시.
		if (SetFireGun && AimmingTime > 1.0f) {
			SetFireGun = false;
		}

		//3초 후 탄환을 발사한다.
		if (AimmingTime > 3.0f) {
			SetFireGun = true;
			FireBullet();
			AimmingTime = 0.0f;
		}

		//플레이어를 조준중인 경우
		if (CanSeePlayer) {
			//플레이어 놓침 타이머가 돌아간다.
			lostPlayerTimer += DeltaTime;
			//플레이어를 놓치고 5초가 지나면 
			if (lostPlayerTimer > 5.0f) {
				//타이머를 리셋하고
				lostPlayerTimer = 0.0f;
				//조준 타이머도 리셋하고
				AimmingTime = 0.0f;
				//플레이어 조준을 해제한다.
				SetCanSeePlayer(false, nullptr);
			}
		}

		if (takeHit) {
			hitTimer += DeltaTime;
			if (hitTimer > 2.0f) {
				hitTimer = 0.0f;
				AimmingTime = 0.0f;
				takeHit = false;
			}
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
	if (health > 0) {
		//시야에 플레이어가 들어올때만 Player가 Nullptr이 아니다
		Player = Cast<ABloodBornCharacter>(PlayerPawn);

		//시야에 플레이어가 들어오면
		if (Player) {
			MakeNoise(1.0f, this, GetActorLocation());
			//SetCanSeePlayer를 실행
			SetCanSeePlayer(true, Player);
		}
	}
}

void ALongRangeEnemy::OnHearNoise(APawn* PlayerPawn, const FVector& Location, float Volume)
{
	ABloodBornCharacter* tempPlayer = Cast<ABloodBornCharacter>(PlayerPawn);
	
	if (!CanSeePlayer) {
		FRotator toward = (Location - GetActorLocation()).Rotation();
		SetActorRotation(toward);
	}
}

void ALongRangeEnemy::SetCanSeePlayer(bool SeePlayer, UObject* isPlayer)
{
	if (health > 0) {
		//시야에 플레이어가 들어왔으면
		if (SeePlayer) {
			//시야에 플레이어가 있는동안 타이머 리셋
			lostPlayerTimer = 0.0f;
			//CanSeePlayer는 스테이트 머신의 조준 애니메이션 발동 트리거
			CanSeePlayer = SeePlayer;
			//플레이어로 캐스팅
			PlayerREF = Cast<ABloodBornCharacter>(isPlayer);
			//플레이어를 향한 백터를 계산. 목적지 - 출발지
			FVector towardPlayer = PlayerREF->GetActorLocation() - GetActorLocation();
			//플레이어를 바라보게 해서 조준하는 것 처럼 보이게 함. Pitch방향은 위아래 방향이므로 액터의 원래 Pitch로 유지
			SetActorRotation(FRotator(GetActorRotation().Pitch, towardPlayer.Rotation().Yaw, towardPlayer.Rotation().Roll));
		}
		else {
			//플레이어 조준 트리거를 해제
			CanSeePlayer = SeePlayer;
		}
	}
}

void ALongRangeEnemy::FireBullet()
{
	//탄환의 발사위치 충돌판정을 세팅
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//탄환을 발사한다
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), (PlayerREF->GetActorLocation() - bulletFirePoint->GetComponentLocation()).Rotation(), params);
}
/*
void ALongRangeEnemy::GotParryAttackCPP(float Damage)
{
	//패리되는 공격이 없으므로 데미지 처리만 호출
	GotDamagedCPP(Damage);
}
*/
void ALongRangeEnemy::GetHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint"));

	// 여긴 걍 Dot 관련 지금은 적 애니메이션도 없고 나중에 필요할지 아닐지도 모르겠음
	// 흠 지금은 애니메이션이 없어서 Theta 각도가 다 0인건가 
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation().GetSafeNormal());

	// Forward * ToHit = | Forward | | ToHit | * cos(theta), | Forward | | ToHit | : normalized벡터
	// | Forward | = 1,  | ToHit | = 1, 그러므로 Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);  //Forward 노멀라이즈해서 길이 1이라 60 곱함
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
		// 여기에 체력바(?)
	}

	GotDamage(DamageAmount);

	return DamageAmount;
}

void ALongRangeEnemy::GotParryAttackCPP(float damage)
{
	GotDamage(damage);
}

void ALongRangeEnemy::GotDamage(float damage)
{
	health -= damage;

	//출혈 이펙트 재생부. 블루프린트의 NiaSys에 미리 파티클을 등록해야한다.
	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator);

	if (health <= 0) {
		PawnSensing->Deactivate();
		AimmingTime = 0.0f;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else {
		takeHit = true;
	}
}