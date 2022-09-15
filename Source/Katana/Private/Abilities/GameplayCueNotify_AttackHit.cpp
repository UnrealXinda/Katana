// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayCueNotify_AttackHit.h"

#include "Kismet/GameplayStatics.h"

UGameplayCueNotify_AttackHit::UGameplayCueNotify_AttackHit()
{
	CameraShakeClass = UCameraShakeBase::StaticClass();
	InnerRadius = 0.0f;
	OuterRadius = 0.0f;
	Falloff = 1.0f;
	bOrientShakeTowardsEpicenter = false;
}

bool UGameplayCueNotify_AttackHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const FVector Center = Parameters.Location;
	UGameplayStatics::PlayWorldCameraShake(MyTarget, CameraShakeClass,
		Center, InnerRadius, OuterRadius, Falloff, bOrientShakeTowardsEpicenter);

	return true;
}