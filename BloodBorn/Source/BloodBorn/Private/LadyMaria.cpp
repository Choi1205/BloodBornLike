

#include "LadyMaria.h"
#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

ALadyMaria::ALadyMaria()
{
	PrimaryActorTick.bCanEverTick = true;

	rightSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSword"));
	rightSword->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightSword->SetCollisionProfileName(TEXT("NoCollision"));

	leftSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSword"));
	leftSword->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	leftSword->SetCollisionProfileName(TEXT("NoCollision"));

	gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gun->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	gun->SetCollisionProfileName(TEXT("NoCollision"));

	rightDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightDamageBox"));
	rightDamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	

	leftDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftDamageBox"));
	leftDamageCollision->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->MaxAcceleration = 200.0f;

}

void ALadyMaria::BeginPlay()
{
	Super::BeginPlay();

	mariaAI = Cast<ALadyMariaAIController>(GetController());
	playerREF = FindPlayer_BP();
	AnimInstance = GetMesh()->GetAnimInstance();

}

void ALadyMaria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//매 틱마다 플레이어와의 거리를 잰다
	distanceToPlayer = FVector::Distance(GetActorLocation(), playerREF->GetActorLocation());

	//공격중이 아니라면 보스는 항상 플레이어를 바라본다.
	if (bIsActing == false) {
		FRotator toward = (playerREF->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(toward);

		//행동을 멈추면 스테미너가 회복된다.
		if (stamina < 1000.0f) {
			//최대치보다 적으면 회복. 일단 초당 50
			stamina += DeltaTime * 50;
		}
		else if (stamina > 1000.0f) {
			//최대치를 넘으면 최대치로 맞춘다.
			stamina = 1000.0f;
		}

	}

	if (mariaAI->bIsRightSlash) {
		RightSlash();
		//UE_LOG(LogTemp, Warning, TEXT("Right, %d"), bIsActing);
	}
	if (mariaAI->bIsLeftSlash) {
		LeftSlash();
		//UE_LOG(LogTemp, Warning, TEXT("Leftt"));
	}
}

ABloodBornCharacter* ALadyMaria::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer형태로 반환한다.
	return Cast<ABloodBornCharacter>(players[0]);
}

void ALadyMaria::WalkToPlayer()
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector towardPlayer = playerLocation - GetActorLocation();

	AddMovementInput(towardPlayer.GetSafeNormal(1.0));
}

//거리 게터 함수
float ALadyMaria::GetPlayerDistance()
{
	return distanceToPlayer;
}
//스테미나 게터 함수
float ALadyMaria::GetBossStamina()
{
	return stamina;
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

void ALadyMaria::GotDamage(float damage)
{
	healthPoint -= damage;
	/*
	//출혈 이펙트 재생부. 블루프린트의 NiaSys에 미리 파티클을 등록해야한다.
	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator);

	if (mariaAI != nullptr) {
		if (healthPoint <= 0) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			mariaAI->PawnSensing->Deactivate();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AnimInstance->Montage_Play(EnemyDyingAnimation);
			mariaAI->UnPossess();
			mariaAI = nullptr;
		}
		else {
			if (!mariaAI->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				mariaAI->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), true);
				AnimInstance->Montage_Play(EnemyHitAnimation);
			}
		}
	}
	*/
}

void ALadyMaria::GotParryAttackCPP(float damage)
{
	GotDamage(damage);
	/*
	if (mariaAI != nullptr) {
		if (CanParryed) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			CanParryed = false;
			mariaAI->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), true);
			mariaAI->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
		}
	}
	*/
	//패리판정 및 스턴판정이 들어감
	UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
}

void ALadyMaria::RightSlash()
{
	//만약 재생중이던 몽타주가 있고
	if (bIsActing && AnimInstance->IsAnyMontagePlaying()) {
		//우측베기가 아니면
		if (!AnimInstance->Montage_IsPlaying(AnimRightSlash)) {
			//몽타주를 정지
			AnimInstance->Montage_Stop(0.0f, NULL);
			//그리고 우측베기를 재생
			AnimInstance->Montage_Play(AnimRightSlash);
			//스테미나 소모
			stamina -= 50.0f;
		}
	}
	//재생중인 몽타주가 없으면
	else if(bIsActing){
		//우측베기를 재생
		AnimInstance->Montage_Play(AnimRightSlash);
		//스테미나 소모
		stamina -= 50.0f;
	}
	
	//몽타주 재생이 끝난 경우
	if (!bIsActing) {
		mariaAI->bIsRightSlash = false;
		if (mariaAI->RandomNextMoveTF(50)) {
			bIsActing = true;
			mariaAI->bIsLeftSlash = true;
		}
	}
}

void ALadyMaria::LeftSlash()
{
	//만약 재생중이던 몽타주가 있고
	if (bIsActing && AnimInstance->IsAnyMontagePlaying()) {
		//우측베기가 아니면
		if (!AnimInstance->Montage_IsPlaying(AnimLeftSlash)) {
			//몽타주를 정지
			AnimInstance->Montage_Stop(0.0f, NULL);
			//그리고 우측베기를 재생
			AnimInstance->Montage_Play(AnimLeftSlash);
			//스테미나 소모
			stamina -= 50.0f;
		}
	}
	//재생중인 몽타주가 없으면
	else if(bIsActing){
		//우측베기를 재생
		AnimInstance->Montage_Play(AnimLeftSlash);
		//스테미나 소모
		stamina -= 50.0f;
	}

	//몽타주 재생이 끝난 경우
	if (!bIsActing) {
		mariaAI->bIsLeftSlash = false;
		if (mariaAI->RandomNextMoveTF(50)) {
			bIsActing = true;
			mariaAI->bIsRightSlash = true;
		}
	}
}
