// Fill out your copyright notice in the Description page of Project Settings.

#include "MedusaAnimInstance.h"
#include "MedusaCharacter.h"

void UMedusaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// Initialize any variables or perform setup here

	MedusaCharacter = Cast<AMedusaCharacter>(TryGetPawnOwner());
	if (MedusaCharacter != nullptr)
	{
		// Set the character movement component reference
		CMC = MedusaCharacter->GetCharacterMovement();
		
	}
}

void UMedusaAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	// Update any animation variables or perform updates here
}
