// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomDangerGenerator.h"

void ARandomDangerGenerator::ActivateGenerator()
{
	Super::ActivateGenerator();
	lastLineDistance = 0;
}

void ARandomDangerGenerator::DeactivateGenerator()
{
	Super::DeactivateGenerator();
}

bool ARandomDangerGenerator::CanDeactivateNow()
{
	return true;
}

TSubclassOf<AObstacle> ARandomDangerGenerator::GetRandomObstacleType()
{
	int index = FMath::RandRange(0, ObstacleTypes.Num() - 1);
	return ObstacleTypes[index];
}

int ARandomDangerGenerator::GetObstaclesPerLine()
{
	float factor = ScaleWithDifficultyLinear(obstaclesPerLineMinFactor, obstaclesPerLineMaxFactor);
	float roadWidth = gameState->GetRoadSize().X;
	int number = (int)(factor * roadWidth);
	return number;
}

float ARandomDangerGenerator::GetDistanceBetweenLines()
{
	float lineDistance = ScaleWithDifficultyLinear(distanceBetweenLinesMax, distanceBetweenLinesMin);
	return lineDistance;
}

float ARandomDangerGenerator::GetDistanceSinceLastLine()
{
	return gameState->GetTravelledDistance() - lastLineDistance;
}

void ARandomDangerGenerator::SpawnLineOfObstacles()
{
	int number = GetObstaclesPerLine();

	FVector2D roadCenter = gameState->GetRoadCenter();
	float roadCenterX = roadCenter.X;
	float roadWidth = gameState->GetRoadSize().X;
	float roadTopZ = gameState->GetRoadTopZ();

	int maxSlots = roadWidth / obstacleWidth;

	for (int i = 0; i < number; i++)
	{
		AObstacle* obstacle = nullptr;
		
		int randSlot = FMath::FRandRange(-maxSlots / 2, maxSlots / 2);
		float x = randSlot * obstacleWidth;
		auto obstacleType = GetRandomObstacleType();
		obstacle = SpawnObstacle(x, roadTopZ, obstacleType);
	}
}

AObstacle* ARandomDangerGenerator::SpawnObstacle(float x, float y, TSubclassOf<AObstacle> obstacleType)
{
	FVector location = FVector(x, 10, y);
	FActorSpawnParameters spawnParams;
	AObstacle* spawnedActor = GetWorld()->SpawnActor<AObstacle>(obstacleType, location, FRotator(), spawnParams);
	if (spawnedActor != nullptr)
	{
#if WITH_EDITOR
		spawnedActor->SetFolderPath("/LevelObjects/Obstacles");
#endif
		UE_LOG(LogTemp, VeryVerbose, TEXT("Spawned obstacle on (%3.2f, %3.2f)"), x, y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn obstacle on (%3.2f, %3.2f)"), x, y);
	}

	return spawnedActor;
}

void ARandomDangerGenerator::Tick(float deltaTime)
{
	if (bIsActive)
	{
		float lineDistance = ScaleWithDifficultyLinear(distanceBetweenLinesMax, distanceBetweenLinesMin);

		float distanceSinceLastLine = GetDistanceSinceLastLine();

		if (distanceSinceLastLine >= lineDistance)
		{
			SpawnLineOfObstacles();

			lastLineDistance = GetTravelledDistance();
		}
	}
}