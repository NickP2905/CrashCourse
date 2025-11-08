// From Druid Mechanics Course


#include "Player/CC_PlayerState.h"
#include  "AbilitySystem/CC_AbilitySystemComponent.h"

ACC_PlayerState::ACC_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	
}

UAbilitySystemComponent* ACC_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
