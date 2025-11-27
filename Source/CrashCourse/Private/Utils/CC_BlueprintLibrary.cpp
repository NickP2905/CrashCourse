// From Druid Mechanics Course


#include "Utils/CC_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Characters/CC_BaseCharacter.h"
#include "Characters/CC_EnemyCharacter.h"
#include "Engine/OverlapResult.h"
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
	FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, UObject* OptionalParticleSystem)
{
	ACC_BaseCharacter* PlayerCharacter = Cast<ACC_BaseCharacter>(Target);
	if (!IsValid(PlayerCharacter))  return;
	if (!PlayerCharacter->IsAlive())  return;

	UCC_AttributeSet* AttributeSet = Cast<UCC_AttributeSet>(PlayerCharacter->GetAttributeSet());
	if (!IsValid(AttributeSet))  return;

	const bool bLethal = AttributeSet->GetHealth() - Damage <= 0.f;
	const FGameplayTag EventTag= bLethal ?  CCTags::Events::Player::Death : CCTags::Events::Player::HitReact;

	Payload.OptionalObject = OptionalParticleSystem;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, EventTag, Payload);

	UAbilitySystemComponent* TargetASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC))  return;

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage );
	
	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

TArray<AActor*> UCC_BlueprintLibrary::HitBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOffset, bool bDrawDebugs)
{
	if (!IsValid(AvatarActor))  return  TArray<AActor*>();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = AvatarActor->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = AvatarActor->GetActorLocation() + Forward + FVector(0.f, 0.f,HitBoxElevationOffset);

	UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))  return  TArray<AActor*>();
	
	World->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& Result : OverlapResults)
	{
		ACC_BaseCharacter* BaseCharacter = Cast<ACC_BaseCharacter>(Result.GetActor());
		if (!IsValid(BaseCharacter))  continue;
		if (!BaseCharacter->IsAlive())  continue;
		ActorsHit.AddUnique(BaseCharacter);
	}
	
	if (bDrawDebugs)
		DrawHitBoxOverlapDebugs(AvatarActor, OverlapResults, HitBoxLocation, HitBoxRadius);

	return ActorsHit;
}

void UCC_BlueprintLibrary::DrawHitBoxOverlapDebugs(const UObject* WorldContextObject,TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, const float HitBoxRadius)
{

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))  return;
	
	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (IsValid(Result.GetActor()))
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 75.f;
			DrawDebugSphere(World, DebugLocation, 30, 10, FColor::Green, false, 3.f);
		}
	}
}

TArray<AActor*> UCC_BlueprintLibrary::ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, float InnerRadius,
	float OuterRadius, float LaunchForceMagnitude, float RotationAngle, bool bDrawDebugs)
{
	for (AActor* HitActor : HitActors)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
		if (!IsValid(HitCharacter) || !IsValid(AvatarActor))  return TArray<AActor*>();

		const FVector HitCharacterLocation = HitActor->GetActorLocation();
		const FVector AvatarLocation = AvatarActor->GetActorLocation();

		const FVector ToHitActor = HitCharacterLocation - AvatarLocation;
		const float Distance = FVector::Dist(AvatarLocation, HitCharacterLocation);

		float LaunchForce = 0.f;
		if (Distance > OuterRadius)  continue;
		if (Distance <= InnerRadius)
		{
			LaunchForce = LaunchForceMagnitude;
		}
		else
		{
			const FVector2D FalloffRange(InnerRadius, OuterRadius);
			const FVector2D LaunchForceRange(LaunchForceMagnitude, 0.f);
			LaunchForce = FMath::GetMappedRangeValueClamped(FalloffRange, LaunchForceRange, Distance);
			if (bDrawDebugs)
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("LaunchForce: %f"), LaunchForce));

			FVector KnockbackForce = ToHitActor.GetSafeNormal();
			KnockbackForce.Z = 0.f;

			const FVector Right = KnockbackForce.RotateAngleAxis(90.f, FVector::UpVector);
			KnockbackForce = KnockbackForce.RotateAngleAxis(-RotationAngle, Right) * LaunchForce;

			if (bDrawDebugs)
			{
				UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
				DrawDebugDirectionalArrow(World, HitCharacterLocation, HitCharacterLocation+KnockbackForce, 100.f, FColor::Green, false, 3.f);
			}
			/**
			if (ACC_EnemyCharacter* EnemyCharacter = Cast<ACC_EnemyCharacter>(HitCharacter) ; IsValid((EnemyCharacter)))
			{
				EnemyCharacter->StopMovementUntilLanded();
			}
			**/
			HitCharacter->LaunchCharacter(KnockbackForce, true, true);
		}
	}
	return HitActors;
}
