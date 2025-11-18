// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "CC_WaitGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class CRASHCOURSE_API UCC_WaitGameplayEvent : public UAbilityAsync_WaitGameplayEvent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Ability|Async", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE"))
	static UCC_WaitGameplayEvent* WaitGameplayEventToActorProxy(AActor* TargetActor, UPARAM(meta=(GameplayTagFilter="GameplayEventTagsCategory")) FGameplayTag EventTag, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);
	


	
	void StartActivation();
};
