// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MedusaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMedusaCharacter : public ACharacter
{
///////////////////////////////////////////
// Input

	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Toggle Debugging Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleDebuggingAction;

protected:
	/** Function callback for  movement input */
	void Move(const FInputActionValue& Value);

	/** Function callback for looking input */
	void Look(const FInputActionValue& Value);

	/** Function callback for Enable Debugging */
	void EnableDebugging();
	
// End Input
/////////////////////////////////////////
	
////////////////////////////////////////
// Debugging

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debugging, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TestArrow1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debugging, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TestArrow2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debugging, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TestArrow3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Debugging, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TestArrow4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging, meta = (AllowPrivateAccess = "true"))
	bool bEnableDebug = false;

// End Debugging 
//////////////////////////////////////////
	
public:
	AMedusaCharacter();

protected:
	
	/** Called when the game starts or when spawned, adds Input Mapping Context */
	virtual void NotifyControllerChanged() override;
	
	/**Function that casts from the PlayerInputComponent to EnhancedInputComponent and sets up bindings*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

///////////////////////////////////
// Getters

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

// End Getters
/////////////////////////////////////////////

};