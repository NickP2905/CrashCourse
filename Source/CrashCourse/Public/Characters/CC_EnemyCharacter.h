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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	UFUNCTION(BlueprintImplementableEvent)
	float GetTimeLineLength();

	void StopMovementUntilLanded();
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	float AcceptanceRadius{500.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	float MinAttackDelay{.1f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	float MaxAttackDelay{.5f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsBeingLaunched{false};

protected:
	
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;

private:

	UFUNCTION()
	void EnableMovementOnLanded(const FHitResult& HitResult);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

};
