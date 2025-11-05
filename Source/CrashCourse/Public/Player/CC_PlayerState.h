// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "CC_PlayerState.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class CRASHCOURSE_API ACC_PlayerState : public APlayerState, public IAbilitySystemInterface  
{
	GENERATED_BODY()
public:
	ACC_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	

private:

	UPROPERTY(VisibleAnywhere, Category="Crash|Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
};
