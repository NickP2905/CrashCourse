#include "GameplayTags/CCTags.h"


namespace CCTags
{
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile, "CCTags.SetByCaller.Projectile", "Tag for set by Caller Magnitude for Projectiles")
	}
	namespace CCAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "CCTags.CCAbilities.Primary", "Tag for Primary Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "CCTags.CCAbilities.Secondary", "Tag for Secondary Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "CCTags.CCAbilities.Tertiary", "Tag for Tertiary Ability");

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "CCTags.CCAbilities.ActivateOnGiven", "Tag for Abilities that should activate immediately once given");

		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "CCTags.CCAbilities.Enemy.Attack", "Enemy Attack Tag");
		}
		
	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "CCTags.Events.KillScored", "Tag for KillScore Event");
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "CCTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "CCTags.Events.Enemy.EndAttack", "Tag for Enemy Ending an Attack");
			
		}
	}
}