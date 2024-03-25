
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BLOODBORN_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
											int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

private:

};
