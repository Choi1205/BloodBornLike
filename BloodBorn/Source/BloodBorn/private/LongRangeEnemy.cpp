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

	//폰 감지 컴포넌트를 생성, 부착
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	//탄환 발사 포인트를 생성, 부착.
	//SetupAttachment에 인자값으로 RootComponent를 주면 C++에서 루트 컴포넌트를 지정하지 않더라도, 자동으로 차일드가 됨.
	bulletFirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletFirePoint"));
	bulletFirePoint->SetupAttachment(RootComponent);
	bulletFirePoint->SetRelativeLocation(FVector(60.0f, 10.0f, 50.0f));

}

// Called when the game starts or when spawned
void ALongRangeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//시야에 들어온 폰이 있으면 OnSeePawn이 작동하도록 바인딩
	PawnSensing->OnSeePawn.AddDynamic(this, &ALongRangeEnemy::OnSeePawn);

}

// Called every frame
void ALongRangeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
}

// Called to bind functionality to input
void ALongRangeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALongRangeEnemy::OnSeePawn(APawn* PlayerPawn)
{
	//시야에 플레이어가 들어올때만 Player가 Nullptr이 아니다
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(PlayerPawn);

	//시야에 플레이어가 들어오면
	if (Player) {
		//SetCanSeePlayer를 실행
		SetCanSeePlayer(true, Player);
		//RunRetriggerableTimer();
		//넣으면 페이탈. 안넣으면 마지막 목격지점으로 총을 계속 쏜다
		/*페이탈 내용
		Fatal error: [File:D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\UObject\ScriptCore.cpp] [Line: 1459] Failed to find function SetCanSeePlayer in BP_LongRangeEnemy_C /Game/Choi/Level/UEDPIE_0_EnemyLevel.EnemyLevel:PersistentLevel.BP_LongRangeEnemy_C_1

		UnrealEditor_CoreUObject
		UnrealEditor_BloodBorn_0111!ALongRangeEnemy::RunRetriggerableTimer() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Source\BloodBorn\private\LongRangeEnemy.cpp:75]

		UnrealEditor_BloodBorn_0111!ALongRangeEnemy::OnSeePawn() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Source\BloodBorn\private\LongRangeEnemy.cpp:58]

		UnrealEditor_BloodBorn_0111!ALongRangeEnemy::execOnSeePawn() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Intermediate\Build\Win64\UnrealEditor\Inc\BloodBorn\UHT\LongRangeEnemy.gen.cpp:26]
		내용으로 미루어 볼 때 델리게이트로 연결한 함수명을 찾지 못하는 것으로 보임. 그런데 함수명은 일치함. 질문하여 확인 필요.
		*/
	}
}

void ALongRangeEnemy::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	//시야에 플레이어가 들어왔으면
	if (SeePlayer) {
		//CanSeePlayer는 스테이트 머신의 조준 애니메이션 발동 트리거
		CanSeePlayer = SeePlayer;
		//플레이어로 캐스팅
		PlayerREF = Cast<ABloodBornCharacter>(Player);
		//플레이어를 향한 백터를 계산. 목적지 - 출발지
		FVector towardPlayer= PlayerREF->GetActorLocation() - GetActorLocation();
		//플레이어를 바라보게 해서 조준하는 것 처럼 보이게 함. Pitch방향은 위아래 방향이므로 액터의 원래 Pitch로 유지
		SetActorRotation(FRotator(GetActorRotation().Pitch, towardPlayer.Rotation().Yaw, towardPlayer.Rotation().Roll));
	}
	else {
		//현재 미작동. RunRetriggerableTimer()를 고쳐야함
		UE_LOG(LogTemp, Warning, TEXT("Exit aim?"));
		CanSeePlayer = SeePlayer;
	}
}

void ALongRangeEnemy::RunRetriggerableTimer()
{
	//시야에서 벗어난 후 플레이어를 추적하는 타이머를 리셋
	//플레이어가 시야에 있으면 계속 이 함수가 호출되서 타이머가 계속 리셋된다.
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	//타이머 종료 후 SetCanSeePlayer()를 실행하도록 바인딩
	FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, nullptr);

	//지정된 시간(PawnSensing->SensingInterval * TrackingPlayer)이 경과하면 바인딩된 함수가 실행됨
	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
}

void ALongRangeEnemy::FireBullet()
{
	//탄환의 발사위치 충돌판정을 세팅
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//탄환을 발사한다
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), GetActorRotation(), params);
}

void ALongRangeEnemy::GotDamagedCPP(float Damage)
{
	//플레이어 HP UI 대응용 -HP방지
	healthPoint -= Damage;
	if (healthPoint < 0) {
		healthPoint = 0;
	}

	//데미지를 받아 경직이 들어가야한다.
	//경직치 처리는 보류
	UE_LOG(LogTemp, Warning, TEXT("Remain HP : %.0f"), healthPoint);

	if (healthPoint == 0) {
		//사망처리부분. 애니메이션 재생 필요
		//Destroy();
	}
}

void ALongRangeEnemy::GotParryAttackCPP(float Damage)
{
	GotDamagedCPP(Damage);
}

