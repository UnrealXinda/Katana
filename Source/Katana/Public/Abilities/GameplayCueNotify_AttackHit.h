// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class KATANA_API UGameplayCueNotify_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float InnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float OuterRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float Falloff;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	bool bOrientShakeTowardsEpicenter;

public:
	UGameplayCueNotify_AttackHit();
	bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};