// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "DefaultPawnBase.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKINGBASICS_API ADefaultPawnBase : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void UpdateTargetLocation(float DeltaTime);
	void UpdateTargetRotation(float DeltaTime);
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void OnFrontalMovement(float Axis);
	void OnSideMovement(float Axis);

	void OnCameraY(float Axis);
	void OnCameraZ(float Axis);

	void LerpPlayerPositionToTarget();
	void LerpPlayerRotationToTarget();
	
	// Client To Server RPCs
	UFUNCTION(Server, Unreliable)
	void Server_UpdateInputVector(const FVector InputVector);

	UFUNCTION(Server, Unreliable)
	void Server_UpdateInputRotation(const FVector InputRotation);

	// Properties
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 270.0f;
	
	// Server Variables
	FVector TargetLocation  = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector ClientMovementInput = FVector::ZeroVector;
	FVector ClientRotationInput = FVector::ZeroVector;
	
	// Client Variables
	FVector CachedMovementInput = FVector::ZeroVector;
	FVector CachedRotationInput = FVector::ZeroVector;
	
	bool IsMovementDirty = false;
	bool IsRotationDirty = false;
};
