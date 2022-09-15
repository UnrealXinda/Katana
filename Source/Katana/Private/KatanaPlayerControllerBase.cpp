// Fill out your copyright notice in the Description page of Project Settings.


#include "KatanaPlayerControllerBase.h"

#include "KatanaCharacterBase.h"

AKatanaPlayerControllerBase::AKatanaPlayerControllerBase()
{
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void AKatanaPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &AKatanaPlayerControllerBase::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AKatanaPlayerControllerBase::MoveRight);

	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &AKatanaPlayerControllerBase::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APlayerController::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AKatanaPlayerControllerBase::LookUpAtRate);

	InputComponent->BindAction("BaseAttack", IE_Pressed, this, &AKatanaPlayerControllerBase::BaseAttack);
	InputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AKatanaPlayerControllerBase::SpecialAttack);
}


void AKatanaPlayerControllerBase::TurnAtRate(float Rate)
{
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AKatanaPlayerControllerBase::LookUpAtRate(float Rate)
{
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AKatanaPlayerControllerBase::MoveForward(float Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		ControlledPawn->AddMovementInput(Direction, Value);
	}
}

void AKatanaPlayerControllerBase::MoveRight(float Value)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		ControlledPawn->AddMovementInput(Direction, Value);
	}
}

void AKatanaPlayerControllerBase::BaseAttack()
{
	if (AKatanaCharacterBase* ControlledCharacter = Cast<AKatanaCharacterBase>(GetPawn()))
	{
		ControlledCharacter->BaseAttack();
	}
}

void AKatanaPlayerControllerBase::SpecialAttack()
{
	if (AKatanaCharacterBase* ControlledCharacter = Cast<AKatanaCharacterBase>(GetPawn()))
	{
		ControlledCharacter->SpecialAttack();
	}
}