// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "NoLobbyGamemode.generated.h"

class APlayerStart;

UCLASS()
class NETWORKINGBASICS_API ANoLobbyGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	APlayerStart* GetPlayerStartForTeam1();
	APlayerStart* GetPlayerStartForTeam2();
	void CachePlayerStarts();

private:
	TArray<AActor*> playerStarts;

	bool spawnedTeam1 = false;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UDataTable* SpawnableCharacters;
};
