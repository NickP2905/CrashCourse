// From Druid Mechanics Course


#include "AbilitySystem/Abilities/Player/CC_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CCTags.h"

TArray<AActor*> UCC_Primary::HitBoxOverlapTest()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0.f, 0.f,HitBoxElevationOffset);

	GetWorld()->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& Result : OverlapResults)
	{
		if (!IsValid(Result.GetActor()))   continue;
		ActorsHit.AddUnique(Result.GetActor());
	}
	
	if (bDrawDebugs)
		DrawHitBoxOverlapDebugs(OverlapResults, HitBoxLocation);

	return ActorsHit;
}

void UCC_Primary::SendHitReactEventToActors(const TArray<AActor*>& HitActors)
{
	for (AActor* HitActor : HitActors)
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, CCTags::Events::Enemy::HitReact, Payload);
	}	
}

void UCC_Primary::DrawHitBoxOverlapDebugs(TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation) const
{
	// DrawDebugSphere(GetWorld(), HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (IsValid(Result.GetActor()))
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 75.f;
			// DrawDebugSphere(GetWorld(), DebugLocation, 30, 10, FColor::Green, false, 3.f);
		}
	}
}
