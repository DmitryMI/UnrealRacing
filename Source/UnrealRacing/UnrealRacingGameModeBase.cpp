// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnrealRacingGameModeBase.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


AUnrealRacingGameModeBase::AUnrealRacingGameModeBase() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

FVector2D AUnrealRacingGameModeBase::GetRoadSize()
{
	return roadSize;
}

void AUnrealRacingGameModeBase::SetupDangerGenerators()
{
	TArray<AActor*> dangerGeneratorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADangerGeneratorBase::StaticClass(), dangerGeneratorActors);

	for (AActor* dangerGeneratorActor : dangerGeneratorActors)
	{
		ADangerGeneratorBase* dangerGenerator = Cast<ADangerGeneratorBase>(dangerGeneratorActor);
		dangerGenerators.Add(dangerGenerator);
	}
}


void AUnrealRacingGameModeBase::BeginPlay()
{
	Super::BeginPlay();	

	gameState = GetGameState<AUnrealRacingGameState>();

	GetWorldTimerManager().SetTimer(raceStartConditionTimer, this, &AUnrealRacingGameModeBase::CheckStartConditionsAndStartRace, 1.0f, true, 2.0f);
}

void AUnrealRacingGameModeBase::CheckStartConditionsAndStartRace()
{
	// Check if all players are connected
	int reservedSlotsLeft = 0;
	int pawnsSpawned = 0;
	for (int i = 0; i < playerControllers.Num(); i++)
	{
		if (playerControllers[i] == nullptr)
		{
			reservedSlotsLeft++;
			continue;
		}

		if (playerControllers[i]->GetPawn() != nullptr)
		{
			pawnsSpawned++;
		}		
	}

	if (reservedSlotsLeft > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckStartConditionsAndStartRace] Reserved slots left: %d. Cannot start."), reservedSlotsLeft);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckStartConditionsAndStartRace] All players connected."));
	}

	if (pawnsSpawned < numberOfPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckStartConditionsAndStartRace] Pawns created: %d/%d. Cannot start."), pawnsSpawned, numberOfPlayers);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CheckStartConditionsAndStartRace] All pawns created."));
	}
	
	SetupDangerGenerators();

	if (gameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[CheckStartConditionsAndStartRace] Game State is nullptr! Cannot start the race."));
	}
	else
	{
		// Notify all clients that race is starting
		gameState->StartRace();
	}

	// Stop condtion checking timer
	GetWorldTimerManager().ClearTimer(raceStartConditionTimer);

	// Start danger generators
	UpdateDangerGenerators();

	UE_LOG(LogTemp, Warning, TEXT("[CheckStartConditionsAndStartRace] Starting race!"));
}

void AUnrealRacingGameModeBase::UpdateDangerGenerators()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateDangerGenerators()"));

	bool hasEnabledGenerators = false;
	for (auto generator : dangerGenerators)
	{
		if (generator->IsGeneratorEnabled())
		{
			hasEnabledGenerators = true;
			break;
		}
	}

	if (hasEnabledGenerators)
	{
		float activityTime = FMath::RandRange(generatorMinTime, generatorMaxTime);
		ADangerGeneratorBase* next = nullptr;
		while (next == nullptr || !next->IsGeneratorEnabled())
		{
			int nextIndex = FMath::RandRange(0, dangerGenerators.Num() - 1);
			next = dangerGenerators[nextIndex];
		}

		if (next == activeGenerator)
		{
			// Same generator, nothing to do
			FString name;
			activeGenerator->GetName(name);
			UE_LOG(LogTemp, Warning, TEXT("Prolonging generator %s for %3.2f"), *name, activityTime);
		}
		else if (activeGenerator != nullptr && activeGenerator->CanDeactivateNow())
		{
			activeGenerator->DeactivateGenerator();
			activeGenerator = next;
			activeGenerator->ActivateGenerator();

			FString name;
			activeGenerator->GetName(name);
			UE_LOG(LogTemp, Warning, TEXT("Activating generator %s for %3.2f"), *name, activityTime);
		}
		else if (activeGenerator == nullptr)
		{
			activeGenerator = next;
			FString name;
			activeGenerator->ActivateGenerator();
			activeGenerator->GetName(name);
			UE_LOG(LogTemp, Warning, TEXT("First generator is %s for %3.2f"), *name, activityTime);
		}

		GetWorldTimerManager().SetTimer(dangerGeneratorUpdateTimer, this, &AUnrealRacingGameModeBase::UpdateDangerGenerators, activityTime, false, activityTime);
	}
	else
	{
		GetWorldTimerManager().SetTimer(dangerGeneratorUpdateTimer, this, &AUnrealRacingGameModeBase::UpdateDangerGenerators, 1.0f, false, 1.0f);
	}
}

void AUnrealRacingGameModeBase::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (gameState == nullptr)
	{
		return;
	}

	if (!gameState->IsRaceStarted())
	{
		return;
	}

}


void AUnrealRacingGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("[InitGame] MapName: %s, Options: %s"), *MapName, *Options);

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("[InitGame] Assuming %d players"), playInEditorNumberOfPlayers);
	numberOfPlayers = playInEditorNumberOfPlayers;
#endif

	// Create Player Start locations
	float roadWidth = roadSize.X;
	float perPlayer = roadWidth / numberOfPlayers;
	float roadLeft = roadCenter.Y - roadWidth / 2;

	for (int i = 0; i < numberOfPlayers; i++)
	{
		float locationX = roadLeft + i * perPlayer + perPlayer / 2;
		float locationY = roadCenter.Y;
		FVector spawnLocation = FVector(locationX, 20, locationY);
		FRotator spawnRotation = FRotator(90, 0, 0);
		FActorSpawnParameters spawnParams;
		spawnParams.bNoFail = true;
		APlayerStart* startSpot = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), spawnLocation, spawnRotation);
		if (startSpot == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("APlayerStart not created!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayerStart created at (%3.2f, %3.2f, %3.2f)"), spawnLocation.X, spawnLocation.Y, spawnLocation.Z);
			playerStartSpots.Add(startSpot);
		}
	}
}

void AUnrealRacingGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	if (!ErrorMessage.IsEmpty())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[PreLogin] Options: %s, Address: %s"), *Options, *Address);

	/*
	if (playerControllers.Num() == numberOfPlayers)
	{
		ErrorMessage = TEXT("Server is full");
		UE_LOG(LogTemp, Warning, TEXT("[PreLogin] Server is full, rejected player: Options: %s, Address: %s"), *Options, *Address);
	}
	else
	{
		// Reserve place for this player
		playerControllers.Add(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("[PreLogin] Place reserved for: Options: %s, Address: %s"), *Options, *Address);
	}
	*/
}

APlayerController* AUnrealRacingGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* login_result = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("[Login] Portal: %s, Options: %s"), *Portal, *Options);
	if (!ErrorMessage.IsEmpty())
	{
		return login_result;
	}

	if (playerControllers.Num() == numberOfPlayers)
	{
		ErrorMessage = TEXT("Server is full");
		UE_LOG(LogTemp, Warning, TEXT("[Login] Server is full, rejected player: Options: %s, Address: %s"), *Options, "N/A");
	}
	else
	{
		ACarPlayerController* carController = Cast<ACarPlayerController>(login_result);
		if (carController == nullptr)
		{
			UE_LOG(LogTemp, Fatal, TEXT("[Login] Player Controller has incompatible type. Player: Options: %s, Address: %s"), *Options, "N/A");
		}
		playerControllers.Add(carController);
		UE_LOG(LogTemp, Warning, TEXT("[Login] Player slot used for: Options: %s, Address: %s"), *Options, "N/A");
	}

	return login_result;
}

void AUnrealRacingGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FString address = NewPlayer->GetPlayerNetworkAddress();
	UE_LOG(LogTemp, Warning, TEXT("[PostLogin] Address: %s"), *address);
}

void AUnrealRacingGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	AActor* StartSpot = nullptr;
	for (int i = 0; i < playerStartSpots.Num() && StartSpot == nullptr; i++)
	{
		AActor* startSpot = playerStartSpots[i];
		if (startSpot->ActorHasTag(FName("Used")))
		{
			continue;
		}
		startSpot->Tags.Add(FName("Used"));
		StartSpot = startSpot;
	}

	// If a start spot wasn't found,
	if (StartSpot == nullptr)
	{
		// Check for a previously assigned spot
		if (NewPlayer->StartSpot != nullptr)
		{
			StartSpot = NewPlayer->StartSpot.Get();
			UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
		}
	}

	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);

	APawn* spawnedPawn = NewPlayer->GetPawn();
	FVector location = StartSpot->GetActorLocation();
	spawnedPawn->SetActorLocation(location);
	FRotator rotation = StartSpot->GetActorRotation();
	spawnedPawn->SetActorRotation(rotation);
}
