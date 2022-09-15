// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_CheckDamage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "KatanaCharacterBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

void UAnimNotify_CheckDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#if WITH_EDITOR
	if (bDebugDraw)
	{
		const UWorld* World = MeshComp->GetWorld();
		const FVector Center = MeshComp->GetComponentTransform().TransformPosition(SphereRelativeLoc);
		constexpr int32 Segments = 32;
		constexpr float Thickness = 1.0f;
		DrawDebugSphere(World, Center, Radius, Segments, FColor::Red, false, DebugDrawDuration, 0, Thickness);
	}
#endif

	AKatanaCharacterBase* Character = Cast<AKatanaCharacterBase>(MeshComp->GetOwner());
	if (!Character)
	{
		return;
	}

	const FVector SphereLoc = MeshComp->GetComponentTransform().TransformPosition(SphereRelativeLoc);
	const TArray<AActor*> ActorsToIgnore = {Character};
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_Pawn)};
	TArray<AActor*> OverlappedActors;

	const bool bOverlapped = UKismetSystemLibrary::SphereOverlapActors(Character, SphereLoc, Radius,
		ObjectTypes, AKatanaCharacterBase::StaticClass(), ActorsToIgnore, OverlappedActors);

	if (bOverlapped)
	{
		for (const AActor* Target : OverlappedActors)
		{
			FGameplayEventData Payload;
			Payload.Instigator = Character;
			Payload.Target = Target;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Character, EventToBroadcast, MoveTemp(Payload));
		}
	}
}