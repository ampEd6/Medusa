// Fill out your copyright notice in the Description page of Project Settings.

#include "MedusaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMedusaCharacter

AMedusaCharacter::AMedusaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create 4 test arrow components
	TestArrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("TestArrow1"));
	TestArrow1->SetupAttachment(GetCapsuleComponent());
	TestArrow1->ArrowColor = FColor::Red;
	
	TestArrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("TestArrow2"));
	TestArrow2->SetupAttachment(GetCapsuleComponent());
	TestArrow2->ArrowColor = FColor::Blue;

	TestArrow3 = CreateDefaultSubobject<UArrowComponent>(TEXT("TestArrow3"));
	TestArrow3->SetupAttachment(GetCapsuleComponent());
	TestArrow3->ArrowColor = FColor::Orange;
	
	TestArrow4 = CreateDefaultSubobject<UArrowComponent>(TEXT("TestArrow4"));
	TestArrow4->SetupAttachment(GetCapsuleComponent());
	TestArrow4->ArrowColor = FColor::Green;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Begin Input


void AMedusaCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMedusaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings and bind them to their respective callback function
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMedusaCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMedusaCharacter::Look);
		// Toggle Debugging
		EnhancedInputComponent->BindAction(ToggleDebuggingAction, ETriggerEvent::Triggered, this, &AMedusaCharacter::EnableDebugging);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMedusaCharacter::EnableDebugging()
{

	if (!bEnableDebug)
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Debugging Enabled"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Debugging Enabled"));

		//Arrow Component Debugging
		//Create local array of all arrow components
		UArrowComponent* ArrowComponents[] = { TestArrow1, TestArrow2, TestArrow3, TestArrow4 };

		//Loop through arrow components, un-hide ones that are in use
		for (UArrowComponent* LocalArrowComponent : ArrowComponents)
		{
			if (LocalArrowComponent->ComponentHasTag(FName("InUse")))
			{
				LocalArrowComponent->SetHiddenInGame(false);
			}
		}
		bEnableDebug = true;
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Debugging Disabled"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Debugging Disabled"));

		//Loop through arrow components, hide them all
		for (UArrowComponent* LocalArrowComponent : { TestArrow1, TestArrow2, TestArrow3, TestArrow4 })
		{
			LocalArrowComponent->SetHiddenInGame(true);
		}
		bEnableDebug = false;
	}
}

void AMedusaCharacter::Move(const FInputActionValue& Value)
{
	// Since the InputActionValue is a struct, we cannot use that information directly so we can use the Get<FVector2D>() function to break the struct and retrieve a Vector2D (XY).
	// We store the Vector2D in case we want to use it later, but we can also use the Get<FVector2D>() function directly in the AddMovementInput() function to access the X and Y values.
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	
	if (Controller != nullptr)
	{
		// Extract only the yaw from the controller's (camera) rotation to calculate movement direction, to ensure movement is only on the XY plane
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	
		// Here we want to move the player in the direction of the camera with respect to the yaw plane (no pitch/roll), so we need to extract the camera's rotation
		// and convert it into a direction (unit vector).
		
		// In order to convert a rotator (camera rotation) into a unit vector (direction to move in), we can use a Rotational Matrix.
		// Accessing the .GetScaledAxis() function, we can feed it a specific axis (X, Y, Z) to get the corresponding unit vector (forward, right, up), respectively.
		// In effect, when we perform this function on the camera, the middle point of where we're looking at is the forward vector, and the right vector is perpendicular to it.
		// Therefore, it's important to zero out the camera's pitch and roll before we convert to a unit vector, otherwise the forward and right vector would not be flat on the XY plane
		// (e.g. if we're looking 30° down, the forward vector would be pointing down as well, and we can't move the player in that direction, only on the XY plane).
		// Once we have the forward and right unit vectors, we can use their direction in the AddMovementInput function, and the character will always move in the direction of the camera's yaw rotation.

		// It's also worth noting that FRotationMatrix::MakeFromX() does the opposite, whereby it takes a unit vector and converts it into a rotator relative to the forward direction (since X = forward).
		// So if the vector (X:0, Y:1, Z:0) is fed into FRotationMatrix::MakeFromX(), it would return a rotator of (P:0, Y:90, R:0).
		// This makes sense because Y axis points to the right, which is equivalent to a 90 degree yaw rotation from the forward direction.

		// It's also worth noting that FRotationMatrix::MakeFrom_() does the opposite, whereby it lets us make a rotator from a unit vector.
		// So we can take a unit vector and convert it into a rotator, which is useful for when we want to see how much rotation is needed to align in the direction of a vector.
		// We can choose two axes as well, and when can input a vector for each axis, and it will orient the object's selected axes toward the vectors we give it.
		// Ex: FMRotationMatrix::MakeFromXY(X vector, Y vector) takes two vectors of our choosing, and it will orient the object's X axis (forward direction) toward the first vector and the Y axis (right direction) toward the second vector.
		// Ex: FRotationMatrix::MakeFromX(FVector(1, 0, 0)) takes one vector of our choosing, and it will return the rotator that aligns the object's X axis (forward direction) the vector we give it, and the Y and Z axes will be perpendicular to the X axis, following the left-hand rule
		// The second example is more straightforward but may cause some flipping since we're only using one axis (there's more than one way to rotate to align the X axis with the vector).
		
		// In short, we can use Rotation Matrices to convert between vectors and rotators.

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

		
		// We add movement in the direction of the camera that we calculated earlier, and we split the Vector2D into the X and Y scalar components to feed as the magnitude (-1 to 1).
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMedusaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// End Input
//////////////////////////////////////////////////////


