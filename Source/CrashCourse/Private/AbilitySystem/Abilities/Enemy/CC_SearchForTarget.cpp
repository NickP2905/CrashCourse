// From Druid Mechanics Course


#include "AbilitySystem/Abilities/Enemy/CC_SearchForTarget.h"
#include "AbilitySystem/Abilities/Enemy/CC_SearchForTarget.h"
#include "AbilitySystem/Abilities/Enemy/CC_SearchForTarget.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "AbilitySystem/AbilityTasks/CC_WaitGameplayEvent.h"
#include "GameplayTags/CCTags.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/CC_BlueprintLibrary.h"

UCC_SearchForTarget::UCC_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	
}

void UCC_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningEnemy = Cast<ACC_EnemyCharacter>(GetAvatarActorFromActorInfo());
	check(OwningEnemy.IsValid());
	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	check(OwningAIController.IsValid());

	StartSearch();

	WaitGameplayEventTask = UCC_WaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(), CCTags::Events::Enemy::EndAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UCC_SearchForTarget::StartSearch()
{


	if (!OwningEnemy.IsValid())  return;
	const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
	SearchDelayTask->Activate();
}

void UCC_SearchForTarget::MoveToTargetAndAttack()
{
	if (bDrawDebugs)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UCC_SearchForTarget::MoveToTargetAndAttack ")));
	if (!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetBaseCharacter.IsValid())   return;
	if (!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}
	MoveToLocationOrActorTask = UAITask_MoveTo::AIMoveTo(OwningAIController.Get(), FVector(), TargetBaseCharacter.Get(),
		OwningEnemy->AcceptanceRadius);
	
	MoveToLocationOrActorTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToLocationOrActorTask->ConditionalPerformMove();
}

void UCC_SearchForTarget::Search()
{
	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	FClosestActorWithTagResult ClosestActorResult =  UCC_BlueprintLibrary::FindClosestActorWithTag(this, SearchOrigin, CrashTags::Player);

	TargetBaseCharacter = Cast<ACC_BaseCharacter>(ClosestActorResult.Actor);

	if (!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}
	if (TargetBaseCharacter->IsAlive())
		MoveToTargetAndAttack();
	else
		StartSearch();
}

void UCC_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	if (OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched)
		StartSearch();
}

void UCC_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{

	// TODO : 36:20 S6L6
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}

	OwningEnemy->RotateToTarget(TargetBaseCharacter.Get());

	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimeLineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
	AttackDelayTask->Activate();
}

void UCC_SearchForTarget::Attack()
{

	const FGameplayTag AttackTag = CCTags::CCAbilities::Enemy::Attack;
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}
