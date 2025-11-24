// From Druid Mechanics Course


#include "Notifies/CC_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetTraceUtils.h"
#include "Characters/CC_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags/CCTags.h"

void UCC_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp))		return;
	if (!IsValid(MeshComp->GetOwner()))	return;

	TArray<FHitResult> Hits = PerformSphereTrace(MeshComp);
	SendEventsToActors(MeshComp, Hits);
	
}

TArray<FHitResult> UCC_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	TArray<FHitResult> OutHits;

	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector Start = SocketTransform.GetLocation();
	const FVector ExtendedSocketDirection = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator()) * SocketExtensionOffset;
	const FVector End = Start - ExtendedSocketDirection;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	
	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))  return OutHits;
	
	bool const bHit = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity,
		ECC_Visibility, FCollisionShape::MakeSphere(SphereTraceRadius), Params, ResponseParams);

	/*
	if (bDrawDebugs)
		DrawDebugSphereTraceMulti(World, Start, End, SphereTraceRadius, EDrawDebugTrace::ForDuration, bHit, OutHits,
			FColor::Red, FColor::Green, 5.f);
	*/

	return OutHits;
}

void UCC_MeleeAttack::SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& HitResults) const
{
	for (const FHitResult& Hit : HitResults)
	{
		ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(Hit.GetActor());
		if (!PlayerCharacter)  continue;
		if (!PlayerCharacter->IsAlive())  continue;
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		if (!IsValid(ASC)) continue;

		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddHitResult(Hit);

		FGameplayEventData PayLoad;
		PayLoad.Target = PlayerCharacter;
		PayLoad.ContextHandle = ContextHandle;
		PayLoad.Instigator = MeshComp->GetOwner();

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), CCTags::Events::Enemy::MeleeTraceHit, PayLoad);
		
	}
}
