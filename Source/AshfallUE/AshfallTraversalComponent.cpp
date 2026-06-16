// Copyright Epic Games, Inc. All Rights Reserved.

#include "AshfallTraversalComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "UObject/UnrealType.h"

UAshfallTraversalComponent::UAshfallTraversalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;  // chequeamos IsFalling cada frame
}

void UAshfallTraversalComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		// SIEMPRE buscar la instancia real al runtime — la asignacion del editor apunta al
		// subobject template, no a la instancia que efectivamente renderiza el arma.
		TArray<USkeletalMeshComponent*> Meshes;
		Char->GetComponents<USkeletalMeshComponent>(Meshes);
		WeaponMesh = nullptr;
		for (USkeletalMeshComponent* Mesh : Meshes)
		{
			if (Mesh && Mesh->GetName().Contains(TEXT("WeaponMesh")))
			{
				WeaponMesh = Mesh;
				break;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("[AshfallTraversal] BeginPlay - WeaponMesh: %s"),
			WeaponMesh ? *WeaponMesh->GetName() : TEXT("NULL"));
	}
}

void UAshfallTraversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!WeaponMesh) return;
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char || !Char->GetCharacterMovement()) return;

	const bool bInAir = Char->GetCharacterMovement()->IsFalling();
	if (bInAir == bWasInAir) return;  // sin cambio, nada que hacer

	// Helper local para setear CurrentWeapon en el AnimInstance via reflection
	auto SetAnimCurrentWeapon = [Char](uint8 NewValue)
	{
		if (USkeletalMeshComponent* CharMesh = Char->GetMesh())
		{
			if (UAnimInstance* AnimInst = CharMesh->GetAnimInstance())
			{
				if (FProperty* Prop = AnimInst->GetClass()->FindPropertyByName(TEXT("CurrentWeapon")))
				{
					if (void* ValuePtr = Prop->ContainerPtrToValuePtr<void>(AnimInst))
					{
						*((uint8*)ValuePtr) = NewValue;
					}
				}
			}
		}
	};

	UE_LOG(LogTemp, Warning, TEXT("[AshfallTraversal] Tick - bInAir cambio a %d"), bInAir);

	if (bInAir)
	{
		// Entrando al aire: guardar estado y ocultar arma + pose desarmada
		bWasVisibleBeforeAir = !WeaponMesh->bHiddenInGame;
		WeaponMesh->SetHiddenInGame(true);
		if (bWasVisibleBeforeAir)
		{
			SetAnimCurrentWeapon(0); // 0 = Unarmed
		}
	}
	else if (bWasVisibleBeforeAir)
	{
		// Aterrizando: restaurar arma + pose con CurrentWeapon real del CBP
		WeaponMesh->SetHiddenInGame(false);
		bWasVisibleBeforeAir = false;
		if (FProperty* CharProp = Char->GetClass()->FindPropertyByName(TEXT("CurrentWeapon")))
		{
			if (const void* CharValuePtr = CharProp->ContainerPtrToValuePtr<void>(Char))
			{
				const uint8 RealValue = *((const uint8*)CharValuePtr);
				SetAnimCurrentWeapon(RealValue);
			}
		}
	}

	bWasInAir = bInAir;
}
