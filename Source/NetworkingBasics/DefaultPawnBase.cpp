// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPawnBase.h"

void ADefaultPawnBase::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation();
	TargetRotation = GetActorRotation();
}

void ADefaultPawnBase::UpdateTargetLocation(float DeltaTime)
{
	FVector ActorForward = GetActorForwardVector();
	FVector ActorRight = GetActorRightVector();

	// Let's Check If This Direction Is Allowed To Be Moved In
	ActorForward *= ClientMovementInput.X;
	ActorRight *= ClientMovementInput.Y;

	// Normalize It In Preparation To Apply The MovementSpeed
	FVector Offset = (ActorForward + ActorRight);
	Offset.Normalize();

	// Offset The TargetLocation
	TargetLocation += Offset * MovementSpeed * DeltaTime;
}

void ADefaultPawnBase::UpdateTargetRotation(float DeltaTime)
{
	FRotator ActorRotation = GetActorRotation();
	FRotator Offset(ClientRotationInput.Y, ClientRotationInput.Z, 0);
	Offset.Roll = 0.0f;
	
	TargetRotation += Offset * RotationSpeed * DeltaTime;
}

void ADefaultPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This should be spawned by the Server therefore only the Server has Authority over this object
	// Server Code	
	if(HasAuthority())
	{
		UpdateTargetLocation(DeltaTime);
		UpdateTargetRotation(DeltaTime);
		
		LerpPlayerPositionToTarget();
		LerpPlayerRotationToTarget();
	}
	else // Client Code
	{
		// We Can Optimize This Further By Ensuring The Tick Is Only Turned On When Either Boolean Is Too Then Turned Off
		if(IsMovementDirty)
		{
			CachedMovementInput.Normalize();
			Server_UpdateInputVector(CachedMovementInput);
			IsMovementDirty = false;
		}

		if(IsRotationDirty)
		{
			CachedRotationInput.Normalize();
			Server_UpdateInputRotation(CachedRotationInput);
			IsRotationDirty = false;
		}
	}
}

void ADefaultPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("FrontalMovement", this, &ADefaultPawnBase::OnFrontalMovement);
	PlayerInputComponent->BindAxis("SideMovement", this, &ADefaultPawnBase::OnSideMovement);

	PlayerInputComponent->BindAxis("CameraY", this, &ADefaultPawnBase::OnCameraY);
	PlayerInputComponent->BindAxis("CameraZ", this, &ADefaultPawnBase::OnCameraZ);
}

void ADefaultPawnBase::OnFrontalMovement(const float Axis)
{
	const float Sign = FMath::Sign(Axis);
	if(CachedMovementInput.X != Sign)
	{
		CachedMovementInput.X = Sign;
		IsMovementDirty = true;
	}
}

void ADefaultPawnBase::OnSideMovement(const float Axis)
{
	const float Sign = FMath::Sign(Axis);
	if(CachedMovementInput.Y != Sign)
	{
		CachedMovementInput.Y = Sign;
		IsMovementDirty = true;
	}
}

void ADefaultPawnBase::OnCameraY(const float Axis)
{
	const float Sign = FMath::Sign(Axis);
	if(CachedRotationInput.Y != Sign)
	{
		CachedRotationInput.Y = Sign;
		IsRotationDirty = true;
	}
}

void ADefaultPawnBase::OnCameraZ(const float Axis)
{
	const float Sign = FMath::Sign(Axis);
	if(CachedRotationInput.Z != Sign)
	{
		CachedRotationInput.Z = Sign;
		IsRotationDirty = true;
	}
}

void ADefaultPawnBase::LerpPlayerPositionToTarget()
{
	SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLocation, 0.6f));
}

void ADefaultPawnBase::LerpPlayerRotationToTarget()
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), TargetRotation, 0.6f));
}

void ADefaultPawnBase::Server_UpdateInputVector_Implementation(const FVector InputVector)
{
	ClientMovementInput = InputVector;
	UE_LOG(LogTemp, Log, TEXT("Got Movement Input: (%f, %f, %f)"), InputVector.X, InputVector.Y, InputVector.Z);
}

void ADefaultPawnBase::Server_UpdateInputRotation_Implementation(const FVector InputRotation)
{
	ClientRotationInput = InputRotation;
	UE_LOG(LogTemp, Log, TEXT("Got Rotation Input: (%f, %f, %f)"), InputRotation.X, InputRotation.Y, InputRotation.Z);
}

