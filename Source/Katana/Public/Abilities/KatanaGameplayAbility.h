// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KatanaGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KATANA_API UKatanaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> EffectClass;
};