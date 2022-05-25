// Fill out your copyright notice in the Description page of Project Settings.


#include "WallGenerator.h"

void AWallGenerator::ActivateGeneratorInternal()
{
	Super::ActivateGeneratorInternal();
	lastLineDistance = 0;
}

void AWallGenerator::DeactivateGenerator()
{
	Super::DeactivateGenerator();
}

bool AWallGenerator::CanDeactivateNow()
{
	return true;
}
int AWallGenerator::GetGateWidthInSlots()
{
	float wallWidth = ScaleWithDifficultyLinear(gateWidthMax, gateWidthMin);
	float wallSlots = wallWidth / obstacleWidth;
	int slotsCeil = FMath::CeilToInt(wallSlots);
	if (slotsCeil < 1)
	{
		return 1;
	}
	return slotsCeil;
}

int AWallGenerator::GetObstaclesPerLine()
{
	float factor = 1.0f;
	float roadWidth = gameState->GetRoadSize().X;
	int number = (int)(factor * roadWidth);
	return number;
}

float AWallGenerator::GetDistanceBetweenLines()
{
	float lineDistance = ScaleWithDifficultyLinear(distanceBetweenLinesMax, distanceBetweenLinesMin);
	return lineDistance;
}

float AWallGenerator::GetDistanceSinceLastLine()
{
	return gameState->GetTravelledDistance() - lastLineDistance;
}

void AWallGenerator::SpawnWallOfObstacles()
{
	FVector2D roadCenter = gameState->GetRoadCenter();
	float roadCenterX = roadCenter.X;
	float roadWidth = gameState->GetRoadSize().X;
	float roadTopZ = gameState->GetRoadTopZ();

	int maxSlots = roadWidth / obstacleWidth;

	int gateWidth = GetGateWidthInSlots();

	int gateStartMin = 0;
	int gateStartMax = maxSlots - gateWidth;

	int gateStart = FMath::RandRange(gateStartMin, gateStartMax);
	int gateEnd = gateStart + gateWidth - 1;

	int leftWallSlots = gateStart + 1;
	int rightWallSlots = maxSlots - gateEnd + 1;

	float debugDrawTime = 1.0f;

	FVector wallMinPos = FVector(roadCenterX - roadWidth / 2.0f, 10, roadTopZ);
	FVector wallMaxPos = FVector(roadCenterX + roadWidth / 2.0f, 10, roadTopZ);
	DrawDebugBox(GetWorld(), wallMinPos, FVector(50, 10, 50), FColor::Green, false, debugDrawTime);
	DrawDebugBox(GetWorld(), wallMaxPos, FVector(50, 10, 50), FColor::Green, false, 1.0f);

	FVector gateStartPos = FVector((gateStart - maxSlots / 2.0) * obstacleWidth, 10, roadTopZ);
	FVector gateEndPos = FVector((gateEnd - maxSlots / 2.0) * obstacleWidth, 10, roadTopZ);
	DrawDebugBox(GetWorld(), gateStartPos, FVector(50, 10, 50), FColor::Red, false, debugDrawTime);
	DrawDebugBox(GetWorld(), gateEndPos, FVector(50, 10, 50), FColor::Red, false, debugDrawTime);

	DrawDebugLine(GetWorld(), wallMinPos, gateStartPos, FColor::Green, false, debugDrawTime, 0, 2.0f);
	DrawDebugLine(GetWorld(), gateEndPos, wallMaxPos, FColor::Green, false, debugDrawTime, 0, 2.0f);
	
	if (leftWallSlots > 0)
	{
		float leftWallX = (wallMinPos.X + gateStartPos.X) / 2.0f;

		FVector wallLocation = FVector(leftWallX, 10, roadTopZ);
		DrawDebugBox(GetWorld(), wallLocation, FVector(50, 10, 50), FColor::Magenta, false, debugDrawTime);

		AWall* leftWall = Cast<AWall>(SpawnObstacle(leftWallX, roadTopZ, WallType));
		if (leftWall != nullptr)
		{
			leftWall->CreateHorizontalWall(leftWallSlots);
		}
	}

	if (rightWallSlots > 0)
	{
		float rightWallX = (wallMaxPos.X + gateEndPos.X) / 2.0f;

		FVector wallLocation = FVector(rightWallX, 10, roadTopZ);
		DrawDebugBox(GetWorld(), wallLocation, FVector(50, 10, 50), FColor::Cyan, false, debugDrawTime);

		AWall* rightWall = Cast<AWall>(SpawnObstacle(rightWallX, roadTopZ, WallType));
		if (rightWall != nullptr)
		{
			rightWall->CreateHorizontalWall(rightWallSlots);
		}
	}
	
}

void AWallGenerator::Tick(float deltaTime)
{
	if (bIsActive && IsGeneratorEnabled())
	{
		float lineDistance = ScaleWithDifficultyLinear(distanceBetweenLinesMax, distanceBetweenLinesMin);

		float distanceSinceLastLine = GetDistanceSinceLastLine();

		if (distanceSinceLastLine >= lineDistance)
		{
			SpawnWallOfObstacles();

			lastLineDistance = GetTravelledDistance();
		}
	}
}