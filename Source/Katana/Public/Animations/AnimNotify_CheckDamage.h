// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CheckDamage.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Check Damage"))
class KATANA_API UAnimNotify_CheckDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag EventToBroadcast;

	UPROPERTY(EditAnywhere, Category = "Combo")
	FVector SphereRelativeLoc;

	UPROPERTY(EditAnywhere, Category = "Combo", meta = (ClampMin = 0.0))
	float Radius = 100.0f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugDraw;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (ClampMin = 0.0))
	float DebugDrawDuration = 1.0f;
#endif

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};