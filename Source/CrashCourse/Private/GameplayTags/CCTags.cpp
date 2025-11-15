#include "GameplayTags/CCTags.h"


namespace CCTags
{
	namespace CCAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "CCTags.CCAbilities.Primary", "Tag for Primary Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "CCTags.CCAbilities.Secondary", "Tag for Secondary Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "CCTags.CCAbilities.Tertiary", "Tag for Tertiary Ability");

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "CCTags.CCAbilities.ActivateOnGiven", "Tag for Abilities that should activate immediately once given");
		
	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "CCTags.Events.KillScored", "Tag for KillScore Event");
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "CCTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event");
			
		}
	}
}