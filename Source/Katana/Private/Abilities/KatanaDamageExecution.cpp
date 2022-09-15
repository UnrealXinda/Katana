// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/KatanaDamageExecution.h"
#include "Abilities/KatanaAttributeSet.h"

struct FKatanaDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FKatanaDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKatanaAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKatanaAttributeSet, Damage, Source, true);
	}

	static const FKatanaDamageStatics& Get()
	{
		static FKatanaDamageStatics DamageStatics;
		return DamageStatics;
	}
};

UKatanaDamageExecution::UKatanaDamageExecution()
{
	RelevantAttributesToCapture.Add(FKatanaDamageStatics::Get().AttackPowerDef);
	RelevantAttributesToCapture.Add(FKatanaDamageStatics::Get().DamageDef);
}

void UKatanaDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FKatanaDamageStatics::Get().AttackPowerDef, EvaluationParameters, AttackPower);

	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FKatanaDamageStatics::Get().DamageDef, EvaluationParameters, Damage);

	const float DamageDone = Damage * AttackPower;
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(FKatanaDamageStatics::Get().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}