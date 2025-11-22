// From Druid Mechanics Course


#include "GameObjects/CC_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/CC_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTags/CCTags.h"
#include "Utils/CC_BlueprintLibrary.h"

ACC_Projectile::ACC_Projectile()
{

	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	bReplicates = true;
}

void ACC_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter))   return;
	if (!PlayerCharacter->IsAlive())   return;
	
	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent) || !HasAuthority())   return;

	FGameplayEventData Payload;
	Payload.Instigator  = GetOwner();
	Payload.Target = PlayerCharacter;

	UCC_BlueprintLibrary::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, CCTags::SetByCaller::Projectile, Damage);

	SpawnImpactEffect();
	
	Destroy();
}


