// From Druid Mechanics Course


#include "CrashCourse/Public/Characters/CC_BaseCharacter.h"


ACC_BaseCharacter::ACC_BaseCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	// Tick & refresh bone transforms whether rendered or not - for bone updates on a dedicated server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
}

UAbilitySystemComponent* ACC_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}



