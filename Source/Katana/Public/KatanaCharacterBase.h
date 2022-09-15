// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "KatanaCharacterBase.generated.h"

class UKatanaGameplayAbility;
class UKatanaAttributeSet;
class UKatanaAbilitySystemComponent;

UCLASS()
class KATANA_API AKatanaCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> BaseAttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> SpecialAttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* BaseHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* SpecialHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;

public:
	AKatanaCharacterBase();
	void BaseAttack();
	void SpecialAttack();
	void PossessedBy(AController* NewController) override;
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
		AKatanaCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleDeath(const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
		AKatanaCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UKatanaGameplayAbility*>& ActiveAbilities) const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	int32 GetNumOfActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags) const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool IsUsingBaseAttack() const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool IsUsingSpecialAttack() const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool IsUsingAbility() const;


protected:
	UPROPERTY()
	UKatanaAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UKatanaAttributeSet* AttributeSet;

	UPROPERTY()
	FGameplayAbilitySpecHandle BaseAttackAbilityHandle;

	UPROPERTY()
	FGameplayAbilitySpecHandle SpecialAttackAbilityHandle;
};