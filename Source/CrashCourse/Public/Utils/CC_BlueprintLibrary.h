// From Druid Mechanics Course

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CC_BlueprintLibrary.generated.h"

struct FGameplayTag;
struct FGameplayEventData;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EHitDirection :uint8 { Left, Right, Forward, Back};

USTRUCT(BlueprintType)
struct FClosestActorWithTagResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;
	UPROPERTY(BlueprintReadWrite)
	float Distance{0.f};
};

/**
 * 
 */
UCLASS()
class CRASHCOURSE_API UCC_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static EHitDirection GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator);

	UFUNCTION(BlueprintPure)
	static FName GetHitDirectionName(const EHitDirection& HitDirection);

	UFUNCTION(BlueprintCallable)
	static FClosestActorWithTagResult FindClosestActorWithTag(const UObject* WorldContextObject, const FVector& Origin, const FName& Tag);

	UFUNCTION(BlueprintCallable)
	static void SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref) FGameplayEventData& Payload,
		const FGameplayTag& DataTag, float Damage, UObject* OptionalParticleSystem = nullptr);
	
	
};
