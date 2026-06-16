// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "AshfallTraversalComponent.generated.h"

class USkeletalMeshComponent;

/**
 * Oculta automaticamente el WeaponMesh cuando el personaje no esta caminando
 * (salto, caida, traversal de GASP). Se suscribe al MovementModeChangedDelegate.
 */
UCLASS(ClassGroup=(Ashfall), meta=(BlueprintSpawnableComponent))
class ASHFALLUE_API UAshfallTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAshfallTraversalComponent();

	/** Malla del arma que se oculta cuando el personaje no esta en suelo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ashfall|Traversal")
	USkeletalMeshComponent* WeaponMesh;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/** Recuerda si el arma estaba visible antes de saltar/caer, para restaurarla al aterrizar */
	bool bWasVisibleBeforeAir = false;

	/** Estado anterior de "en el aire" para detectar cambio */
	bool bWasInAir = false;
};
