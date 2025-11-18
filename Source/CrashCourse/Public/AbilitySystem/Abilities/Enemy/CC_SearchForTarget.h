// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "Characters/CC_EnemyCharacter.h"
#include "CC_SearchForTarget.generated.h"


class UAITask_MoveTo;
class UCC_WaitGameplayEvent;
class UAbilityTask_WaitDelay;
class ACC_BaseCharacter;

namespace EPathFollowingResult
{
	enum Type : int;
	
}

/**
 * 
 */
UCLASS()
class CRASHCOURSE_API UCC_SearchForTarget : public UCC_GameplayAbility
{
	GENERATED_BODY()

public:
	UCC_SearchForTarget();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	TWeakObjectPtr<ACC_EnemyCharacter>  OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;
	TWeakObjectPtr<ACC_BaseCharacter>  TargetBaseCharacter;


private:

	UPROPERTY()
	TObjectPtr<UCC_WaitGameplayEvent> WaitGameplayEventTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> SearchDelayTask;

	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToLocationOrActorTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;
	
	void StartSearch();
	void MoveToTargetAndAttack();

	UFUNCTION()
	void Search();
	UFUNCTION()
	void Attack();

	UFUNCTION()
	void EndAttackEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result , AAIController* AIController);
};

