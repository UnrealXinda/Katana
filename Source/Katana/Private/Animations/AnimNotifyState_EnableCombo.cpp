// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_EnableCombo.h"

#include "KatanaCharacterBase.h"
#include "Abilities/KatanaAbilitySystemComponent.h"

void UAnimNotifyState_EnableCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	const AKatanaCharacterBase* Character = Cast<AKatanaCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		UKatanaAbilitySystemComponent* ASC = Cast<UKatanaAbilitySystemComponent>(Character->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->NextComboName = EnabledComboName;
			ASC->bAcceptComboInput = true;
		}
	}
}

void UAnimNotifyState_EnableCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_EnableCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const AKatanaCharacterBase* Character = Cast<AKatanaCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		UKatanaAbilitySystemComponent* ASC = Cast<UKatanaAbilitySystemComponent>(Character->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->NextComboName = NAME_None;
			ASC->bAcceptComboInput = false;
		}
	}
}