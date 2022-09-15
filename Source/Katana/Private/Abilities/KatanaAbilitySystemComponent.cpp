// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/KatanaAbilitySystemComponent.h"
#include "Abilities/KatanaGameplayAbility.h"

namespace
{
	void IterateActiveAbilitiesWithTags(const UAbilitySystemComponent& Component, const FGameplayTagContainer& GameplayTagContainer,
		TFunctionRef<void(UGameplayAbility*)> Func)
	{
		TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
		Component.GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

		// Iterate the list of all ability specs
		for (const FGameplayAbilitySpec* Spec : AbilitiesToActivate)
		{
			// Iterate all instances on this ability spec
			TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				Func(ActiveAbility);
			}
		}
	}
}

void UKatanaAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer,
	TArray<UKatanaGameplayAbility*>& ActiveAbilities) const
{
	IterateActiveAbilitiesWithTags(*this, GameplayTagContainer, [&ActiveAbilities](UGameplayAbility* ActiveAbility)
	{
		ActiveAbilities.Add(Cast<UKatanaGameplayAbility>(ActiveAbility));
	});
}

int32 UKatanaAbilitySystemComponent::GetNumOfActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer) const
{
	int32 Count = 0;
	IterateActiveAbilitiesWithTags(*this, GameplayTagContainer, [&Count](UGameplayAbility* ActiveAbility)
	{
		if (ActiveAbility && ActiveAbility->IsA<UKatanaGameplayAbility>())
		{
			++Count;
		}
	});

	return Count;
}