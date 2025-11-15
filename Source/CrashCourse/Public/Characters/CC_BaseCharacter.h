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

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCIniitialised, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class CRASHCOURSE_API ACC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACC_BaseCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const {return nullptr;}

	bool IsAlive() const { return bAlive; }
	void SetAlive(bool bAliveStatus) {bAlive = bAliveStatus;}

	UFUNCTION(BlueprintCallable, Category = "Crash|Death")
	virtual void HandleRespawn();

	UFUNCTION(BlueprintCallable, Category = "Crash|Attributes")
	void ResetAttributes();

	UPROPERTY(BlueprintAssignable)
	FASCIniitialised OnASCInitialised;
	
protected:
	void GiveStartupAbilities();
	void InitialiseAttributes() const;

	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();
private:

	UPROPERTY(EditDefaultsOnly, Category="Crash|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Crash|Effects")
	TSubclassOf<UGameplayEffect> InitialiseAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category="Crash|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Replicated)
	bool bAlive = true;
	
};
