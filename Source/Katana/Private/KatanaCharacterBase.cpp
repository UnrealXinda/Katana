// Fill out your copyright notice in the Description page of Project Settings.


#include "KatanaCharacterBase.h"

#include "Abilities/KatanaAbilitySystemComponent.h"
#include "Abilities/KatanaAttributeSet.h"

namespace
{
	FName GetRandomSectionName(const UAnimMontage& Montage)
	{
		const int32 Count = Montage.CompositeSections.Num();
		const int32 RandomIdx = FMath::RandRange(0, Count - 1);
		return Montage.CompositeSections[RandomIdx].SectionName;
	}

	void PlayAnimMontageRandomSection(ACharacter* Character, UAnimMontage* Montage, float Rate = 1.0f)
	{
		if (Montage)
		{
			const FName SectionName = GetRandomSectionName(*Montage);
			Character->PlayAnimMontage(Montage, Rate, SectionName);
		}
	}
}

AKatanaCharacterBase::AKatanaCharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UKatanaAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UKatanaAttributeSet>(TEXT("AttributeSet"));
}

void AKatanaCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(AbilitySystemComponent)
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (BaseAttackAbility)
	{
		BaseAttackAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BaseAttackAbility));
	}
	if (SpecialAttackAbility)
	{
		SpecialAttackAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SpecialAttackAbility));
	}
}

UAbilitySystemComponent* AKatanaCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AKatanaCharacterBase::BaseAttack()
{
	const UKatanaAbilitySystemComponent* ASC = Cast<UKatanaAbilitySystemComponent>(AbilitySystemComponent);
	const bool bCanBaseAttack = !IsUsingAbility() || ASC->bAcceptComboInput;
	if (bCanBaseAttack)
	{
		AbilitySystemComponent->TryActivateAbility(BaseAttackAbilityHandle);
	}
}

void AKatanaCharacterBase::SpecialAttack()
{
	const UKatanaAbilitySystemComponent* ASC = Cast<UKatanaAbilitySystemComponent>(AbilitySystemComponent);
	const bool bCanSpecialAttack = !IsUsingAbility() || ASC->bAcceptComboInput;
	if (bCanSpecialAttack)
	{
		AbilitySystemComponent->TryActivateAbility(SpecialAttackAbilityHandle);
	}
}

void AKatanaCharacterBase::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags,
                                                      TArray<UKatanaGameplayAbility*>& ActiveAbilities) const
{
	AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
}

int32 AKatanaCharacterBase::GetNumOfActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags) const
{
	return AbilitySystemComponent->GetNumOfActiveAbilitiesWithTags(AbilityTags);
}

bool AKatanaCharacterBase::IsUsingBaseAttack() const
{
	static const FGameplayTag BaseAttackTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.BaseAttack"));
	static const FGameplayTagContainer BaseAttackTags{BaseAttackTag};
	const int32 Count = GetNumOfActiveAbilitiesWithTags(BaseAttackTags);
	return Count > 0;
}

bool AKatanaCharacterBase::IsUsingSpecialAttack() const
{
	static const FGameplayTag SpecialAttackTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.SpecialAttack"));
	static const FGameplayTagContainer SpecialAttackTags{SpecialAttackTag};
	const int32 Count = GetNumOfActiveAbilitiesWithTags(SpecialAttackTags);
	return Count > 0;
}

bool AKatanaCharacterBase::IsUsingAbility() const
{
	return IsUsingBaseAttack() || IsUsingSpecialAttack();
}

void AKatanaCharacterBase::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, AKatanaCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	static const FGameplayTag BaseAttackTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.BaseAttack"));
	static const FGameplayTag SpecialAttackTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.SpecialAttack"));

	UAnimMontage* MontageToPlay = nullptr;
	if (BaseAttackTag.MatchesAnyExact(DamageTags))
	{
		MontageToPlay = BaseHitMontage;
	}
	else if (SpecialAttackTag.MatchesAnyExact(DamageTags))
	{
		MontageToPlay = SpecialHitMontage;
	}

	PlayAnimMontageRandomSection(this, MontageToPlay);
}

void AKatanaCharacterBase::HandleDeath(const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
	AKatanaCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	PlayAnimMontageRandomSection(this, DeathMontage);
}