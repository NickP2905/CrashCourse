// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "CC_BaseCharacter.h"
#include "CC_EnemyCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class CRASHCOURSE_API ACC_EnemyCharacter : public ACC_BaseCharacter
{
	GENERATED_BODY()

public:
	
	ACC_EnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

};
