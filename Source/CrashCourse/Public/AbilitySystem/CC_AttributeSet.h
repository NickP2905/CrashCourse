// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CC_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributesInitialised);

/**
 * 
 */
UCLASS()
class CRASHCOURSE_API UCC_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintAssignable)
	FAttributesInitialised OnAttributesInitialised;

	UPROPERTY(ReplicatedUsing = OnRep_AttributesInitialised)
	bool bAttributesInitialised = false;
	UFUNCTION()
	void OnRep_AttributesInitialised();

	//  Health

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData& OldValue);

	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	// MaxHealth

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UFUNCTION()
	void OnRep_MaxHealth(FGameplayAttributeData& OldValue);

	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	// Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;

	UFUNCTION()
	void OnRep_Mana(FGameplayAttributeData& OldValue);

	ATTRIBUTE_ACCESSORS(ThisClass, Mana);

	// MaxMana

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;

	UFUNCTION()
	void OnRep_MaxMana(FGameplayAttributeData& OldValue);

	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);
	
};
