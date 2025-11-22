// From Druid Mechanics Course


#include "Utils/CC_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Characters/CC_BaseCharacter.h"
#include "GameplayTags/CCTags.h"
#include "Kismet/GameplayStatics.h"

EHitDirection UCC_BlueprintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float Dot = FVector::DotProduct(TargetForward, ToInstigator);
	if (Dot < -0.5f)
		return EHitDirection::Back;
	if (Dot < 0.5f)
	{
		// either Lef or Right
		const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);
		if (Cross.Z < 0.f)
			return EHitDirection::Left;
		return EHitDirection::Right;
	}
	return EHitDirection::Forward;
	
}

FName UCC_BlueprintLibrary::GetHitDirectionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
		case EHitDirection::Back: return FName("Back");
		case EHitDirection::Left: return FName("Left");
		case EHitDirection::Right: return FName("Right");
		case EHitDirection::Forward: return FName("Forward");

		default: return FName("None");
	}
}

FClosestActorWithTagResult UCC_BlueprintLibrary::FindClosestActorWithTag(const UObject* WorldContextObject,
	const FVector& Origin, const FName& Tag)
{
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, Tag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor))  continue;
		ACC_BaseCharacter* BaseCharacter = Cast<ACC_BaseCharacter>(Actor);
		if (!IsValid(BaseCharacter) || !BaseCharacter->IsAlive())  continue;
		
		const float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestActor = Actor;
			ClosestDistance = Distance;
		}
	}
	FClosestActorWithTagResult Result;
	Result.Actor = ClosestActor;
	Result.Distance = ClosestDistance;
	return Result;
}

void UCC_BlueprintLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect,
	const FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage)
{
	ACC_BaseCharacter* PlayerCharacter = Cast<ACC_BaseCharacter>(Target);
	if (!IsValid(PlayerCharacter))  return;
	if (!PlayerCharacter->IsAlive())  return;

	UCC_AttributeSet* AttributeSet = Cast<UCC_AttributeSet>(PlayerCharacter->GetAttributeSet());
	if (!IsValid(AttributeSet))  return;

	const bool bLethal = AttributeSet->GetHealth() - Damage <= 0.f;
	const FGameplayTag EventTag= bLethal ?  CCTags::Events::Player::Death : CCTags::Events::Player::HitReact;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, EventTag, Payload);

	UAbilitySystemComponent* TargetASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC))  return;

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage );
	
	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
