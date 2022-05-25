// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	//SetReplicateMovement(true);

	movementComponent = CreateDefaultSubobject<UObstacleMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	gameState = GetWorld()->GetGameState<AUnrealRacingGameState>();
	
	if (GetWorld()->GetNetMode() != NM_Client)
	{
		// Server-only code
		gameMode = GetWorld()->GetAuthGameMode<AUnrealRacingGameModeBase>();		
	}
	else
	{
		// Client-only code
	}
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

