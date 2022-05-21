// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DangerGeneratorBase.h"
#include "CarPlayerController.h"
#include "MousePlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "UnrealRacingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API AUnrealRacingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	int playInEditorNumberOfPlayers = 2;
#endif

	UPROPERTY(EditDefaultsOnly)
	float generatorMinTime = 5;

	UPROPERTY(EditDefaultsOnly)
	float generatorMaxTime = 15;

	UPROPERTY(EditDefaultsOnly)
	FVector2D roadSize = FVector2D(1000, 2000);
	UPROPERTY(EditDefaultsOnly)
	FVector2D roadCenter = FVector2D(0, 0);

	UPROPERTY(EditDefaultsOnly)
	float raceStartConditionCheckPeriod = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float raceStartConditionTimeout = 120.0f;

	UPROPERTY(BlueprintReadOnly, Transient)
	int numberOfPlayers;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<ACarPlayerController*> playerControllers;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<APlayerStart*> playerStartSpots;

	ADangerGeneratorBase* activeGenerator;
	TArray<ADangerGeneratorBase*> dangerGenerators;
	FTimerHandle dangerGeneratorUpdateTimer;

	AUnrealRacingGameState* gameState;

	FTimerHandle raceStartConditionTimer;
	void CheckStartConditionsAndStartRace();
	
	void SetupDangerGenerators();
	virtual void BeginPlay() override;

	void UpdateDangerGenerators();
	virtual void Tick(float deltaTime) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreLogin
	(
		const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	virtual APlayerController* Login
	(
		UPlayer* NewPlayer,
		ENetRole InRemoteRole,
		const FString& Portal,
		const FString& Options,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	virtual void PostLogin
	(
		APlayerController* NewPlayer
	) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

public:
	AUnrealRacingGameModeBase();
	
	UFUNCTION(BlueprintCallable)
	FVector2D GetRoadSize();
};
