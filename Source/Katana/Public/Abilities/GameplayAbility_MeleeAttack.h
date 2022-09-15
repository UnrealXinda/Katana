// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/KatanaGameplayAbility.h"
#include "GameplayAbility_MeleeAttack.generated.h"

class UAnimMontage;

/**
 * A gameplay ability that triggers a montage to play and wait for gameplay events
 * with given tags to trigger. Then it will apply gameplay effects and play gameplay cues
 */
UCLASS()
class KATANA_API UGameplayAbility_MeleeAttack : public UKatanaGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* Montage;

	// Event to check damage
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	FGameplayTagContainer TagsToWait;

	UPROPERTY(EditDefaultsOnly, Category = "Cue")
	FGameplayTag CueTag;

public:
	UGameplayAbility_MeleeAttack();
	void PlayGameplayCue();
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void HandleEndAbility(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void HandleEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};