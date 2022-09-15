// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_MeleeAttack.h"
#include "Abilities/AbilityTask_PlayMontageAndWaitForEvent.h"
#include "Abilities/KatanaAbilitySystemComponent.h"

UGameplayAbility_MeleeAttack::UGameplayAbility_MeleeAttack()
{
	static const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Montage"));
	TagsToWait = FGameplayTagContainer{EventTag};
}

void UGameplayAbility_MeleeAttack::PlayGameplayCue()
{
	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (ASC && AvatarActor)
	{
		FGameplayCueParameters Param;
		Param.Location = AvatarActor->GetActorLocation();
		K2_ExecuteGameplayCueWithParams(CueTag, Param);
	}
}

void UGameplayAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	const UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const UKatanaAbilitySystemComponent* TargetASC = Cast<UKatanaAbilitySystemComponent>(ASC);
	if (TargetASC)
	{
		const FName SectionName = TargetASC->NextComboName;

		UAbilityTask_PlayMontageAndWaitForEvent* Task = UAbilityTask::NewAbilityTask<UAbilityTask_PlayMontageAndWaitForEvent>(this, NAME_None);
		Task->MontageToPlay = Montage;
		Task->EventTags = TagsToWait;
		Task->StartSection = SectionName;
		Task->bStopWhenAbilityEnds = true;

		Task->OnBlendOut.AddDynamic(this, &UGameplayAbility_MeleeAttack::HandleEndAbility);
		Task->OnInterrupted.AddDynamic(this, &UGameplayAbility_MeleeAttack::HandleEndAbility);
		Task->OnCancelled.AddDynamic(this, &UGameplayAbility_MeleeAttack::HandleEndAbility);
		Task->EventReceived.AddDynamic(this, &UGameplayAbility_MeleeAttack::HandleEventReceived);

		Task->Activate();
	}
}

void UGameplayAbility_MeleeAttack::HandleEndAbility(FGameplayTag EventTag, FGameplayEventData EventData)
{
	K2_EndAbility();
}

void UGameplayAbility_MeleeAttack::HandleEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (!EventTag.MatchesAnyExact(TagsToWait))
	{
		return;
	}

	TArray<FHitResult> HitResults;
	TArray<AActor*> TargetActors;

	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		HitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		TargetActors.Add(const_cast<AActor*>(EventData.Target));
	}

	FGameplayAbilityTargetDataHandle TargetData;
	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		TargetData.Add(NewData);
	}

	constexpr float Level = 0.0f;
	const FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(EffectClass, Level);
	K2_ApplyGameplayEffectSpecToTarget(EffectHandle, TargetData);

	PlayGameplayCue();
}