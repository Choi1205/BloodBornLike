

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
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	smokeActor = GetWorld()->SpawnActor<ASmokeFXActor>(smokeVFX, FVector::ZeroVector, FRotator::ZeroRotator, params);
}

void ALadyMaria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�� ƽ���� �÷��̾���� �Ÿ��� ���
	distanceToPlayer = FVector::Distance(GetActorLocation(), playerREF->GetActorLocation());

	playerSpeed =  playerREF->GetVelocity().Length();

	//�������� �ƴ϶�� ������ �׻� �÷��̾ �ٶ󺻴�.
	if (bIsActing == false) {
		FRotator toward = (playerREF->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(toward);

		//�ൿ�� ���߸� ���׹̳ʰ� ȸ���ȴ�.
		if (stamina < 1000.0f) {
			//�ִ�ġ���� ������ ȸ��. �ϴ� �ʴ� 50
			stamina += DeltaTime * 50;
		}
		else if (stamina > 1000.0f) {
			//�ִ�ġ�� ������ �ִ�ġ�� �����.
			stamina = 1000.0f;
		}

	}

	if (mariaAI->bIsFireGun) {
		FireGun();
	}
	if (mariaAI->bIsThrust) {
		Thrust();
	}
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
}

ABloodBornCharacter* ALadyMaria::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer���·� ��ȯ�Ѵ�.
	return Cast<ABloodBornCharacter>(players[0]);
}

//�Ÿ� ���� �Լ�
float ALadyMaria::GetPlayerDistance()
{
	return distanceToPlayer;
}
float ALadyMaria::GetPlayerSpeed()
{
	return playerSpeed;
}
//���׹̳� ���� �Լ�
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
		//150 �Ÿ� �����̸� �Ʒ� ��� �ӵ��� �÷��̾� �ֺ��� ����. ���� ���� �� ���������ϋ� �ֺ� �̵���
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
		// ���⿡ ü�¹�(?)
	}

	GotDamage(DamageAmount);

	return DamageAmount;
}

//��ݰ��� ������ ó���� �ʿ��ϱ� ������ ü���� �����ϴ� �Լ��� ���� �ۼ�
void ALadyMaria::GotDamage(float damage)
{
	//HP�� ������ ��ŭ ��´�
	healthPoint -= damage;

	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));
	if (mariaAI != nullptr) {
		if (healthPoint <= 0) {
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
			if (!mariaAI->bIsStun || !bIsSuperArmor) {
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
	//��ݰ��� ȸ��üũ. ȸ���� 50%
	if (!bIsActing && mariaAI->RandomNextMoveTF(50)) {
		//������ ON���¿����� ����.
		mariaAI->bIsForwardDodge = false;
		//ȸ�� ���� ����
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
		//�и����� �� ���������� ��
		UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
	}
}

void ALadyMaria::RightSlash()
{
	//���� ������̴� ��Ÿ�ְ� �ְ�
	if (bIsActing && AnimInstance->IsAnyMontagePlaying()) {
		//�������Ⱑ �ƴϸ�
		if (!AnimInstance->Montage_IsPlaying(AnimRightSlash)) {
			//��Ÿ�ָ� ����
			AnimInstance->Montage_Stop(0.0f, NULL);
			//�׸��� �������⸦ ���
			AnimInstance->Montage_Play(AnimRightSlash);
			//���׹̳� �Ҹ�
			stamina -= 50.0f;
		}
	}
	//������� ��Ÿ�ְ� ������
	else if (bIsActing) {
		//�������⸦ ���
		AnimInstance->Montage_Play(AnimRightSlash);
		//���׹̳� �Ҹ�
		stamina -= 50.0f;
	}
	
	//��Ÿ�� ����� ���� ���
	if (!bIsActing) {
		mariaAI->bIsRightSlash = false;
		if (mariaAI->RandomNextMoveTF(50) && distanceToPlayer < 150.0f) {
			bIsActing = true;
			mariaAI->bIsLeftSlash = true;
		}
	}
}

void ALadyMaria::LeftSlash()
{
	//���� ������̴� ��Ÿ�ְ� �ְ�
	if (bIsActing && AnimInstance->IsAnyMontagePlaying()) {
		//�������Ⱑ �ƴϸ�
		if (!AnimInstance->Montage_IsPlaying(AnimLeftSlash)) {
			//��Ÿ�ָ� ����
			AnimInstance->Montage_Stop(0.0f, NULL);
			//�׸��� �������⸦ ���
			AnimInstance->Montage_Play(AnimLeftSlash);
			//���׹̳� �Ҹ�
			stamina -= 50.0f;
		}
	}
	//������� ��Ÿ�ְ� ������
	else if(bIsActing){
		//�������⸦ ���
		AnimInstance->Montage_Play(AnimLeftSlash);
		//���׹̳� �Ҹ�
		stamina -= 50.0f;
	}

	//��Ÿ�� ����� ���� ���
	if (!bIsActing) {
		mariaAI->bIsLeftSlash = false;
		if (mariaAI->RandomNextMoveTF(50) && distanceToPlayer < 150.0f) {
			bIsActing = true;
			mariaAI->bIsRightSlash = true;
		}
	}
}

void ALadyMaria::Thrust()
{
	//���� ������̴� ��Ÿ�ְ� �ְ�
	if (bIsActing && AnimInstance->IsAnyMontagePlaying()) {
		//��Ⱑ �ƴϸ�
		if (!AnimInstance->Montage_IsPlaying(AnimDualThrust)) {
			//��Ÿ�ָ� ����
			AnimInstance->Montage_Stop(0.0f, NULL);
			//�׸��� ��⸦ ���
			AnimInstance->Montage_Play(AnimDualThrust);
			//���׹̳� �Ҹ�
			stamina -= 100.0f;
		}
	}
	//������� ��Ÿ�ְ� ������
	else if (bIsActing) {
		//��⸦ ���
		AnimInstance->Montage_Play(AnimDualThrust);
		//���׹̳� �Ҹ�
		stamina -= 100.0f;
	}


	//��Ÿ�� ����� ���� ���
	if (!bIsActing) {
		mariaAI->bIsThrust = false;
	}
}

void ALadyMaria::FireGun()
{
	//��Ÿ������� �ߵ��ǹǷ�, �ൿ���� �ƴϰ�, ������� ��Ÿ�ְ� ���� ���(ȸ������ �ƴѰ��)
	if (!bIsActing && !AnimInstance->IsAnyMontagePlaying()) {
		bIsActing = true;
		AnimInstance->Montage_Play(AnimGunShot);
	}
	//źȯ�� �߻�� ������ �ִԳ�Ƽ���̸� �̿��Ͽ� �ٽ� ����ȴ�.
	else {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//źȯ�� �߻��Ѵ�
		ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), (playerREF->GetActorLocation() - bulletFirePoint->GetComponentLocation()).Rotation(), params);
		if (bullet != nullptr) {
			bullet->SetBulletSpeed(2000.0f);
			bullet->SetFirePower(40.0f);
		}
	}

	//��Ÿ�� ����� ���� ���
	if (!bIsActing) {
		mariaAI->bIsFireGun = false;
	}
}
