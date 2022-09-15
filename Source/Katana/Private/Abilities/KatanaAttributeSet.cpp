// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/KatanaAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "KatanaCharacterBase.h"

UKatanaAttributeSet::UKatanaAttributeSet() :
	Health(100.f),
	MaxHealth(100.f),
	AttackPower(10.0f),
	Damage(0.0f)
{
}

void UKatanaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AKatanaCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<AKatanaCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AKatanaCharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (!SourceController && !SourceActor)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<AKatanaCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<AKatanaCharacterBase>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			const float NewHealth = FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth());
			SetHealth(NewHealth);

			if (TargetCharacter)
			{
				if (OldHealth == 0.0f && NewHealth == 0.0f)
				{
					return;
				}
				if (OldHealth > 0.0f && NewHealth == 0.0f)
				{
					TargetCharacter->HandleDeath(HitResult, SourceTags, SourceCharacter, SourceActor);
				}
				else
				{
					TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
				}
			}
		}
	}
}