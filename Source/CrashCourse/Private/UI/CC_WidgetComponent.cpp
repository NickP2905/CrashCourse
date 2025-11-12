// From Druid Mechanics Course


#include "UI/CC_WidgetComponent.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/CC_BaseCharacter.h"
#include "UI/CC_AttributeWidget.h"


void UCC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilitySystemData();

	if (!IsASCInitialised())
	{
		CrashCharacter->OnASCInitialised.AddDynamic(this, &ThisClass::OnASCInitialised);
		return;
	}

	InitialiseAttributeDelegate();
		
}

void UCC_WidgetComponent::InitAbilitySystemData()
{
	CrashCharacter = Cast<ACC_BaseCharacter>(GetOwner());
	AttributeSet = Cast<UCC_AttributeSet>(CrashCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(CrashCharacter->GetAbilitySystemComponent());
	
}

bool UCC_WidgetComponent::IsASCInitialised() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCC_WidgetComponent::InitialiseAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialised)
	{
		AttributeSet->OnAttributesInitialised.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}

void UCC_WidgetComponent::OnASCInitialised(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UCC_AttributeSet>(AS);

	if (!IsASCInitialised())  return;
	InitialiseAttributeDelegate();
	
}

void UCC_WidgetComponent::BindToAttributeChanges()
{
	//  TODO:
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair);		// for checking the owned widget object

		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, Pair);
		});

		// Otherwise, loop over all children on the owned widget, and do the same foo them
	}
}


void UCC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	// Check the user widget object owned by this component, if its a CC_AttributeWidget, handle it
	UCC_AttributeWidget* AttributeWidget = Cast<UCC_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget))  return;		// We only care about CC Attribute Widgets
	if (!AttributeWidget->MatchesAttributes(Pair))  return;		// Only subscribe for matching Attributes 

	AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get());		// Initial values

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair] (const FOnAttributeChangeData& AttributeChangeData)
	{
		AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get());		// For changes during game
	});
		
	
}
