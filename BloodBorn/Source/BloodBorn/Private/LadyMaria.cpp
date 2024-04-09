

#include "LadyMaria.h"
#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "BulletActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "SmokeFXActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ALadyMaria::ALadyMaria()
{
	PrimaryActorTick.bCanEverTick = true;

	rightSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSword"));
	rightSword->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightSword->SetCollisionProfileName(TEXT("NoCollision"));

	rightReverseSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightReverseSword"));
	rightReverseSword->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightReverseSword->SetCollisionProfileName(TEXT("NoCollision"));
	rightReverseSword->SetVisibility(false);

	leftSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSword"));
	leftSword->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	leftSword->SetCollisionProfileName(TEXT("NoCollision"));

	gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gun->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	gun->SetCollisionProfileName(TEXT("NoCollision"));
	gun->SetVisibility(false);

	rightDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightDamageBox"));
	rightDamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	leftDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftDamageBox"));
	leftDamageCollision->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	bulletFirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletFirePoint"));
	bulletFirePoint->SetupAttachment(gun);
	bulletFirePoint->SetRelativeLocation(FVector(38.0f, 0.0f, 7.0f));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->MaxAcceleration = 200.0f;

}

void ALadyMaria::BeginPlay()
{
	Super::BeginPlay();

	mariaAI = Cast<ALadyMariaAIController>(GetController());
	playerREF = FindPlayer_BP();
	AnimInstance = GetMesh()->GetAnimInstance();

	rightDamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadyMaria::OnDealDamageOverlapBegin);
	leftDamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadyMaria::OnDealDamageOverlapBegin);

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	smokeActor1 = GetWorld()->SpawnActor<ASmokeFXActor>(smokeVFX, FVector::ZeroVector, FRotator::ZeroRotator, params);
	smokeActor2 = GetWorld()->SpawnActor<ASmokeFXActor>(smokeVFX, FVector::ZeroVector, FRotator::ZeroRotator, params);
}

void ALadyMaria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//매 틱마다 플레이어와의 거리를 잰다
	distanceToPlayer = FVector::Distance(GetActorLocation(), playerREF->GetActorLocation());
	//플레이어의 속도도 잰다
	playerSpeed =  playerREF->GetVelocity().Length();

	if (!phase2Cheaker && mariaAI->bIsChangeMode && phase == 0) {
		if (mariaAI->bIsDualSword) {
			leftSword->SetVisibility(true);
			gun->SetVisibility(false);
			rightReverseSword->SetVisibility(false);
		}
		else{
			leftSword->SetVisibility(false);
			gun->SetVisibility(true);
			rightReverseSword->SetVisibility(true);
		}
		mariaAI->bIsChangeMode = false;
	}
	else if(!phase2Cheaker && phase > 0){
		phase2Cheaker = true;
	}

	//공격중이 아니라면 보스는 항상 플레이어를 바라본다.
	if (bIsActing == false) {
		FRotator toward = (playerREF->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(toward);

		//행동을 멈추면 스테미너가 회복된다.
		if (stamina < 1000.0f) {
			//최대치보다 적으면 회복. 페이즈에 따라 회복량 변화
			stamina += DeltaTime * staminaRegain;
		}
		else if (stamina > 1000.0f) {
			//최대치를 넘으면 최대치로 맞춘다.
			stamina = 1000.0f;
		}

	}
	/*
	if (mariaAI->bIsThrust) {
		Thrust();
	}
	*/

	if (mariaAI->bIsRightSlash) {
		RightSlash();
	}
	if (mariaAI->bIsLeftSlash) {
		LeftSlash();
	}
	
	if(bIsMovingWhileAttack){
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 400);
	}
	if(bIsAimmingWhileAttack){
		SetActorRotation((playerREF->GetActorLocation() - GetActorLocation()).Rotation());
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s"), bIsSuperArmor?*FString("True"):*FString("False"));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), mariaAI->bIsStun? *FString("True") : *FString("False"));
}

ABloodBornCharacter* ALadyMaria::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer형태로 반환한다.
	return Cast<ABloodBornCharacter>(players[0]);
}

//거리 게터 함수
float ALadyMaria::GetPlayerDistance()
{
	return distanceToPlayer;
}
float ALadyMaria::GetPlayerSpeed()
{
	return playerSpeed;
}
//스테미나 게터 함수
float ALadyMaria::GetBossStamina()
{
	return stamina;
}

void ALadyMaria::WalkToPlayer()
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector towardPlayer = playerLocation - GetActorLocation();

	if (distanceToPlayer > 150.0f) {
		AddMovementInput(towardPlayer.GetSafeNormal(1.0));
	}
	else {
		//150 거리 안쪽이면 아래 상수 속도로 플레이어 주변을 돈다. 공격 동료 후 근접상태일떄 주변 이동용
		FVector sidemoveDir = GetActorRightVector() * 5;
		if (mariaAI->bIsLeftMove) {
			sidemoveDir *= -1;
		}
		AddMovementInput(sidemoveDir.GetSafeNormal(1.0));
	}
}

void ALadyMaria::ForwardDodge()
{
	bIsActing = true;
	AnimInstance->Montage_Play(AnimDodgeForward);
}

void ALadyMaria::GetHit(const FVector& ImpactPoint)
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

float ALadyMaria::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

//사격공격 데미지 처리도 필요하기 때문에 체력이 감소하는 함수를 따로 작성
void ALadyMaria::GotDamage(float damage)
{
	//HP를 데미지 만큼 깍는다
	healthPoint -= damage;

	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));
	if (mariaAI != nullptr) {
		if (healthPoint <= 0) {
			bIsCanDealDamage = false;

			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AnimInstance->Montage_Play(AnimBossDying);
			AnimInstance->Montage_SetPlayRate(AnimBossDying, 1.0f);
			mariaAI->UnPossess();
			mariaAI = nullptr;
		}
		else {
			if (!mariaAI->bIsStun && !bIsSuperArmor) {
				bIsCanDealDamage = false;
				UE_LOG(LogTemp, Warning, TEXT("Hit!"));

				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				bIsActing = true;
				bIsMovingWhileAttack = false;
				mariaAI->bIsRightSlash = false;
				mariaAI->bIsLeftSlash = false;
				AnimInstance->Montage_Play(AnimBossHit);
				AnimInstance->Montage_SetPlayRate(AnimBossHit, 1.0f);
			}
		}
	}
}

void ALadyMaria::GotParryAttackCPP(float damage)
{
	//사격공격 회피체크. 회피율 50%
	if (!bIsActing && mariaAI->RandomNextMoveTF(50)) {
		//전방대시 ON상태였으면 끈다.
		mariaAI->bIsForwardDodge = false;
		//회피 방향 판정
		if (mariaAI->RandomNextMoveTF(50)) {
			mariaAI->bIsLeftMove = true;
			AnimInstance->Montage_Play(AnimDodgeLeft);
		}
		else {
			mariaAI->bIsLeftMove = false;
			AnimInstance->Montage_Play(AnimDodgeRight);
		}
	}
	else {
		GotDamage(damage);
		
		if (mariaAI != nullptr) {
			if (bIsCanParryed) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				bIsCanParryed = false;
				mariaAI->bIsStun = true;
			}
		}
		
		//패리판정 및 스턴판정이 들어감
		UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
	}
}

void ALadyMaria::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (Player && bIsCanDealDamage) {//플레이어이고, 공격도 가능하다면
		//플레이어는 데미지를 입었다.
		if (mariaAI -> bIsThrust) {
			UGameplayStatics::ApplyDamage(Player, 100, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
		else {
			UGameplayStatics::ApplyDamage(Player, 50, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
	}
}

void ALadyMaria::RightSlash()
{
	//만약 재생중이던 몽타주가 있고
	if (AnimInstance->IsAnyMontagePlaying()) {
		//우측베기가 아니면
		if (!AnimInstance->Montage_IsPlaying(AnimRightSlash)) {
			//몽타주를 정지
			AnimInstance->Montage_Stop(0.0f, NULL);
		}
	}
	else {
		//그리고 우측베기를 재생
		AnimInstance->Montage_Play(AnimRightSlash);
		//스테미나 소모
		stamina -= 50.0f;
		bIsActing = true;
	}
}

void ALadyMaria::LeftSlash()
{
	//만약 재생중이던 몽타주가 있고
	if (AnimInstance->IsAnyMontagePlaying()) {
		//좌측베기가 아니면
		if (!AnimInstance->Montage_IsPlaying(AnimLeftSlash)) {
			//몽타주를 정지
			AnimInstance->Montage_Stop(0.0f, NULL);
		}
	}
	else {
		//그리고 좌측베기를 재생
		AnimInstance->Montage_Play(AnimLeftSlash);
		//스테미나 소모
		stamina -= 50.0f;
		bIsActing = true;
	}
}

void ALadyMaria::Thrust()
{
	//만약 재생중이던 몽타주가 있고
	if (AnimInstance->IsAnyMontagePlaying()) {
		//찌르기가 아니면
		if (!AnimInstance->Montage_IsPlaying(AnimDualThrust)) {
			//몽타주를 정지
			AnimInstance->Montage_Stop(0.0f, NULL);
		}
	}
	//찌르기를 재생
	AnimInstance->Montage_Play(AnimDualThrust);
	//스테미나 소모
	stamina -= 100.0f;
	bIsActing = true;
}

void ALadyMaria::AimGun()
{
	//장거리에서만 발동되므로, 행동중이 아니고, 재생중인 몽타주가 없는 경우(회피중이 아닌경우)
	if (!bIsActing && !AnimInstance->IsAnyMontagePlaying()) {
		bIsActing = true;
		AnimInstance->Montage_Play(AnimGunShot);
	}
}

void ALadyMaria::FireGun()
{
	
	//탄환이 발사될 순간에 애님노티파이를 이용하여 다시 실행된다.
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//탄환을 발사한다
	ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), (playerREF->GetActorLocation() - bulletFirePoint->GetComponentLocation()).Rotation(), params);
	if (bullet != nullptr) {
		bullet->SetBulletSpeed(2000.0f);
		bullet->SetFirePower(40.0f);
	}
}

void ALadyMaria::ABP_AttackStart()
{
	if(mariaAI->bIsThrust){
		bIsSuperArmor = true;
		UE_LOG(LogTemp, Warning, TEXT("True"));
	}
	else{
		AnimInstance->Montage_SetPlayRate(NULL, 0.1f);
		UE_LOG(LogTemp, Warning, TEXT("False"));
	}
	bIsAimmingWhileAttack = true;
}

void ALadyMaria::ABP_SlowEnd()
{
	if (mariaAI->bIsThrust) {
		AnimInstance->Montage_SetPlayRate(AnimDualThrust, 2.0f);
	}
	else{
		AnimInstance->Montage_SetPlayRate(NULL, 1.0f);
	}
	bIsCanDealDamage = true;
	bIsAimmingWhileAttack = false;
}

void ALadyMaria::ABP_AttackEnd()
{
	bIsActing = false;
	if (mariaAI->RandomNextMoveTF(50)) {
		mariaAI->bIsLeftMove = true;
	}
	else{
		mariaAI->bIsLeftMove = false;
	}

	if (mariaAI->RandomNextMoveTF(30)) {
		mariaAI->bIsForwardDodge = true;
		UE_LOG(LogTemp, Warning, TEXT("ForwardDodge is True"));
	}
	else {
		mariaAI->bIsForwardDodge = false;
		UE_LOG(LogTemp, Warning, TEXT("ForwardDodge is False"));
	}

	mariaAI->bIsRightSlash = false;
	mariaAI->bIsLeftSlash = false;
	mariaAI->bIsFireGun = false;
	mariaAI->bIsThrust = false;
	bIsSuperArmor = false;
}

void ALadyMaria::ABP_DodgeStart()
{
	GetMesh()->SetVisibility(false, true);
	smokeActor1->SetActorLocation(GetActorLocation());
	smokeActor1->PlayFX();
}

void ALadyMaria::ABP_DodgeEnd()
{
	if(mariaAI->bIsForwardDodge) {
		FVector playerToMaria = (GetActorLocation() - playerREF->GetActorLocation()).GetSafeNormal() * 100.0f;
		SetActorLocation(playerREF->GetActorLocation() + playerToMaria);
		GetMesh()->SetVisibility(true, true);
		bIsActing = false;
		mariaAI->bIsForwardDodge = false;
	}
	else{
		FVector nextLoc = GetActorRightVector();
		if(mariaAI->bIsLeftMove){
			nextLoc *= -1.0f;
		}
		nextLoc = GetActorLocation() + nextLoc * 200.0f;

		SetActorLocation(nextLoc);
		GetMesh()->SetVisibility(true, true);
		AnimInstance->Montage_SetPlayRate(NULL, 0.25f);
		bIsActing = false;
	}
	smokeActor2->SetActorLocation(GetActorLocation());
	smokeActor2->PlayFX();
}