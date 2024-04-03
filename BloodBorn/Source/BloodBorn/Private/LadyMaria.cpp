

#include "LadyMaria.h"
#include "LadyMariaAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

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

}

void ALadyMaria::BeginPlay()
{
	Super::BeginPlay();

	mariaAI = Cast<ALadyMariaAIController>(GetController());

}

void ALadyMaria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector towardPlayer = playerLocation - GetActorLocation();

	FVector randomLocation = BTAIController->GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));
	FVector nextMovePoint = randomLocation - GetActorLocation();
	*/
	//1페이즈, 플레이어와 거리가 멀고, 공격명령이 없는 상태일때
	if (true) {
		//AddMovementInput(nextMovePoint.GetSafeNormal(1.0));
		//SetActorRotation(towardPlayer.Rotation());
	}
	
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