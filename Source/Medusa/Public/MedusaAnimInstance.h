// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MedusaAnimInstance.generated.h"

/**
 *  Forward declarations
 */

UCLASS()
class MEDUSA_API UMedusaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, Category = "References")
	class AMedusaCharacter* MedusaCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UCharacterMovementComponent* CMC;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float GroundSpeed;
};


