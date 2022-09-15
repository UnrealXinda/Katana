// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "KatanaDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class KATANA_API UKatanaDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UKatanaDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};