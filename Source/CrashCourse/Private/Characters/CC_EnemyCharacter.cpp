// From Druid Mechanics Course


#include "Characters/CC_EnemyCharacter.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"


ACC_EnemyCharacter::ACC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ACC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent()))  return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	if (!HasAuthority())  return;

	GiveStartupAbilities();
	
}

