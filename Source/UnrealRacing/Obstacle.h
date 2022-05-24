// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealRacingGameModeBase.h"
#include "UnrealRacingGameState.h"
#include "ObstacleMovementComponent.h"
#include "Obstacle.generated.h"

UCLASS()
class UNREALRACING_API AObstacle : public AActor
{
	GENERATED_BODY()

protected:
	AUnrealRacingGameModeBase* gameMode;
	AUnrealRacingGameState* gameState;
	UObstacleMovementComponent* movementComponent;
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
