// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "CC_BaseCharacter.generated.h"

class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCIniitialised, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class CRASHCOURSE_API ACC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACC_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const {return nullptr;}

	UPROPERTY(BlueprintAssignable)
	FASCIniitialised OnASCInitialised;
	
protected:
	void GiveStartupAbilities();
	void InitialiseAttributes() const;
	
private:

	UPROPERTY(EditDefaultsOnly, Category="Crash|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Crash|Effects")
	TSubclassOf<UGameplayEffect> InitialiseAttributesEffect;
	
};
