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

	TArray<int> occupiedSlots;

	for (int i = 0; i < number; i++)
	{
		if (occupiedSlots.Num() == maxSlots)
		{
			break;
		}

		AObstacle* obstacle = nullptr;
		int randSlot;
		do
		{
			randSlot = FMath::FRandRange(0, maxSlots);
		} while (occupiedSlots.Contains(randSlot));

		occupiedSlots.Add(randSlot);

		float x = (randSlot - maxSlots / 2.0f) * obstacleWidth;
		auto obstacleType = GetRandomObstacleType();
		obstacle = Cast<AObstacle>(SpawnObstacle(x, roadTopZ, obstacleType));
	}
}

void ARandomDangerGenerator::Tick(float deltaTime)
{
	if (bIsActive && IsGeneratorEnabled())
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