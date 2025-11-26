// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "CC_Primary.generated.h"

/**
 * 
 */
UCLASS()
class CRASHCOURSE_API UCC_Primary : public UCC_GameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Crash|Abilities")
	void SendHitReactEventToActors(const TArray<AActor*>& HitActors);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Abilities")
	float HitBoxRadius = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Abilities")
	float HitBoxForwardOffset = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Crash|Abilities")
	float HitBoxElevationOffset = 75.f;	



	
};
