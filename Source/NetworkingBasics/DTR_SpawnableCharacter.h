// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "DTR_SpawnableCharacter.generated.h"


USTRUCT(Blueprintable)
struct FSpawnableCharacter : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APawn> Pawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;
};
