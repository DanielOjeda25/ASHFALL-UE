// Copyright Epic Games, Inc. All Rights Reserved.

#include "AshfallUECharacter.h"
#include "AshfallUEProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAshfallUECharacter

AAshfallUECharacter::AAshfallUECharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// El cuerpo gira con el yaw del controller (necesario para apuntar en 3ra persona)
	bUseControllerRotationYaw = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Velocidad base de caminata
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Carga el Input Action de sprint por codigo (sin asignarlo en el Blueprint)
	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(TEXT("/Game/FirstPerson/Input/Actions/IA_Sprint.IA_Sprint"));
	if (SprintActionFinder.Succeeded())
	{
		SprintAction = SprintActionFinder.Object;
	}

	// --- Tercera persona (toggle estilo Serious Sam) ---
	// Cuerpo completo (malla heredada del Character): se ve en 3ra persona
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshFinder(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (BodyMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(BodyMeshFinder.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> BodyAnimFinder(TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny"));
	if (BodyAnimFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BodyAnimFinder.Class);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetOwnerNoSee(true); // arranca en 1ra persona: el dueño no ve su propio cuerpo

	// Spring arm + cámara de 3ra persona
	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
	ThirdPersonSpringArm->SetupAttachment(GetCapsuleComponent());
	ThirdPersonSpringArm->TargetArmLength = 300.f;
	ThirdPersonSpringArm->SocketOffset = FVector(0.f, 40.f, 70.f); // sobre el hombro
	ThirdPersonSpringArm->bUsePawnControlRotation = true;

	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCameraComponent->bUsePawnControlRotation = false;
	ThirdPersonCameraComponent->bAutoActivate = false; // arranca en 1ra persona

	// Input action del toggle (cargado por código, sin asignar en el Blueprint)
	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleViewFinder(TEXT("/Game/FirstPerson/Input/Actions/IA_ToggleView.IA_ToggleView"));
	if (ToggleViewFinder.Succeeded())
	{
		ToggleViewAction = ToggleViewFinder.Object;
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AAshfallUECharacter::NotifyControllerChanged()
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

void AAshfallUECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAshfallUECharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAshfallUECharacter::Look);

		// Sprint (Shift)
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AAshfallUECharacter::StartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AAshfallUECharacter::StopSprint);
		}

		// Toggle 1ra/3ra persona
		if (ToggleViewAction)
		{
			EnhancedInputComponent->BindAction(ToggleViewAction, ETriggerEvent::Started, this, &AAshfallUECharacter::ToggleView);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AAshfallUECharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAshfallUECharacter::Look(const FInputActionValue& Value)
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

void AAshfallUECharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AAshfallUECharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AAshfallUECharacter::ToggleView()
{
	bIsThirdPerson = !bIsThirdPerson;
	ApplyViewMode();
}

void AAshfallUECharacter::ApplyViewMode()
{
	if (bIsThirdPerson)
	{
		// 3ra persona: cámara detrás, se ve el cuerpo, se ocultan los brazos 1P
		ThirdPersonCameraComponent->SetActive(true);
		FirstPersonCameraComponent->SetActive(false);
		Mesh1P->SetVisibility(false);
		GetMesh()->SetOwnerNoSee(false);
	}
	else
	{
		// 1ra persona: cámara en la cabeza, brazos 1P visibles, cuerpo oculto al dueño
		FirstPersonCameraComponent->SetActive(true);
		ThirdPersonCameraComponent->SetActive(false);
		Mesh1P->SetVisibility(true);
		GetMesh()->SetOwnerNoSee(true);
	}
}