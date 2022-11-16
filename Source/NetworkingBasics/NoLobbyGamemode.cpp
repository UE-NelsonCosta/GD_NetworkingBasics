// Fill out your copyright notice in the Description page of Project Settings.


#include "NoLobbyGamemode.h"

#include "DTR_SpawnableCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

FString ANoLobbyGamemode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                        const FString& Options, const FString& Portal)
{
	APlayerStart* playerStart = GetPlayerStartForTeam1();
	if(spawnedTeam1)
	{
		playerStart = GetPlayerStartForTeam2();
	}

	UE_LOG(LogTemp, Warning, TEXT("Picked StartPointNamed: %s"), *playerStart->GetName());

	
	// Spawn An Actor
	UClass* classToSpawn = SpawnableCharacters->FindRow<FSpawnableCharacter>("Cone", "")->Pawn;
	if(spawnedTeam1)
	{
		classToSpawn = SpawnableCharacters->FindRow<FSpawnableCharacter>("Sphere", "")->Pawn;
	}

	
	FVector location = playerStart->GetActorLocation();
	FRotator rotation = playerStart->GetActorRotation();
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	APawn* spawnedActor = Cast<APawn>(GetWorld()->SpawnActor(classToSpawn, &location, &rotation, spawnParams));
	// Give Ownership If It's Required

	NewPlayerController->ClientSetLocation (location, rotation);
	
	NewPlayerController->Possess(spawnedActor);
	spawnedActor->SetOwner(NewPlayerController);

	spawnedTeam1 = true;

	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

APlayerStart* ANoLobbyGamemode::GetPlayerStartForTeam1()
{
	if(playerStarts.Num() == 0)
		CachePlayerStarts();
	
	for(int i = 0; i < playerStarts.Num(); ++i)
	{
		if(playerStarts[i]->Tags.Contains("Team1"))
		{
			return Cast<APlayerStart>(playerStarts[i]);
		}
	}

	return nullptr;
}

APlayerStart* ANoLobbyGamemode::GetPlayerStartForTeam2()
{
	if(playerStarts.Num() == 0)
		CachePlayerStarts();
	
	for(int i = 0; i < playerStarts.Num(); ++i)
	{
		if(playerStarts[i]->Tags.Contains("Team2"))
		{
			return Cast<APlayerStart>(playerStarts[i]);
		}
	}

	return nullptr;
}

void ANoLobbyGamemode::CachePlayerStarts()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
}
