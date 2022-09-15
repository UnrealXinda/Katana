// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KatanaAbilitySystemComponent.generated.h"

class UKatanaGameplayAbility;

/**
 * 
 */
UCLASS()
class KATANA_API UKatanaAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combo")
	FName NextComboName;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combo")
	bool bAcceptComboInput;

public:
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UKatanaGameplayAbility*>& ActiveAbilities) const;
	int32 GetNumOfActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer) const;
};