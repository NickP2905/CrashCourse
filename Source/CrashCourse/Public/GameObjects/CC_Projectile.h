// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CC_Projectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS()
class CRASHCOURSE_API ACC_Projectile : public AActor
{
	GENERATED_BODY()

public:
	
	ACC_Projectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Crash|Projectile")
	void SpawnImpactEffect();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|Damage", meta = (ExposeOnSpawn, ClampMin = "0.1"))
	float Damage{-15.f};

private:

	UPROPERTY(VisibleAnywhere, Category="Crash|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category="Crash|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
