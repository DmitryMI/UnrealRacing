// Fill out your copyright notice in the Description page of Project Settings.


#include "ZigzagGenerator.h"

float AZigzagGenerator::GetPivotDistance()
{
	float distance = ScaleWithDifficultyLinear(intervalLenghtMax, intervalLenghtMax);
	return distance;
}


float AZigzagGenerator::GetRandomRoadX()
{
	FVector2D roadCenter = gameState->GetRoadCenter();
	FVector2D roadSize = gameState->GetRoadSize();

	float xMin = roadCenter.X - roadSize.X / 2;
	float xMax = roadCenter.X + roadSize.X / 2;

	float x = FMath::RandRange(xMin, xMax);
	return x;
}

float AZigzagGenerator::GetGateWidth()
{
	return ScaleWithDifficultyLinear(gateWidthMax, gateWidthMin);
}

void AZigzagGenerator::SpawnSegment()
{
	float toNextPivot = nextPivotDistance - GetTravelledDistance();
	float fromPreviousPivot = GetTravelledDistance() - (nextPivotDistance - distanceToNextPivotBase);

	float lerpAlpha = 1.0f - toNextPivot / distanceToNextPivotBase;

	float directionDot = FVector2D::DotProduct(previousIntervalDirection, currentIntervalDirection);

	float clearZoneSize = (1.0f - directionDot) * GetGateWidth() / 2.0f + obstacleSize.Y;

	if (fromPreviousPivot < clearZoneSize)
	{
		return;
	}

	float segmentX = FMath::Lerp(previosPivot, nextPivot, lerpAlpha);
	
	FVector2D segmentDirection = GetNextPivotLocation() - GetPreviosPivotLocation();
	segmentDirection.Normalize();

	float gateWidth = GetGateWidth();
	FVector2D leftObstacleLocation = FVector2D(segmentX, gameState->GetRoadTopZ()) + segmentDirection.GetRotated(90) * gateWidth / 2.0f;
	FVector2D rightObstacleLocation = FVector2D(segmentX, gameState->GetRoadTopZ()) + segmentDirection.GetRotated(-90) * gateWidth / 2.0f;

	DrawDebugBox(GetWorld(), FVector(leftObstacleLocation.X, 10, leftObstacleLocation.Y), FVector(25, 25, 25), FColor::Cyan, false, 0.5f);
	DrawDebugBox(GetWorld(), FVector(rightObstacleLocation.X, 10, rightObstacleLocation.Y), FVector(25, 25, 25), FColor::Cyan, false, 0.5f);

	SpawnObstacle(leftObstacleLocation.X, leftObstacleLocation.Y, ObstacleType);
	SpawnObstacle(rightObstacleLocation.X, rightObstacleLocation.Y, ObstacleType);
}


float AZigzagGenerator::GetPreviosPivotZ()
{
	float previosPivotDistance = nextPivotDistance - distanceToNextPivotBase;
	float previosPivotZ = gameState->GetRoadTopZ() - GetTravelledDistance() + previosPivotDistance;
	return previosPivotZ;
}

float AZigzagGenerator::GetNextPivotZ()
{
	float nextPivotZ = gameState->GetRoadTopZ() - GetTravelledDistance() + nextPivotDistance;
	return nextPivotZ;
}

FVector2D AZigzagGenerator::GetPreviosPivotLocation()
{
	return FVector2D(previosPivot, GetPreviosPivotZ());
}

FVector2D AZigzagGenerator::GetNextPivotLocation()
{
	return FVector2D(nextPivot, GetNextPivotZ());
}

void AZigzagGenerator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!bIsActive || !IsGeneratorEnabled())
	{
		return;
	}

	if (GetMeasuredDistance() >= obstacleSize.Y)
	{
		SpawnSegment();
		StartDistanceMeasuring();
	}

	if (GetTravelledDistance() >= nextPivotDistance)
	{
		previosPivot = nextPivot;
		nextPivot = GetRandomRoadX();

		previousIntervalDirection = currentIntervalDirection;
		currentIntervalDirection = GetNextPivotLocation() - GetPreviosPivotLocation();
		currentIntervalDirection.Normalize();		

		distanceToNextPivotBase = GetPivotDistance();
		nextPivotDistance = GetTravelledDistance() + distanceToNextPivotBase;
	}


	DrawDebugLine(GetWorld(), FVector(previosPivot, 10, GetPreviosPivotZ()), FVector(nextPivot, 10, GetNextPivotZ()), FColor::Green);
}

void AZigzagGenerator::ActivateGeneratorInternal()
{
	ADangerGeneratorBase::ActivateGeneratorInternal();

	distanceToNextPivotBase = GetPivotDistance();
	nextPivotDistance = GetTravelledDistance() + distanceToNextPivotBase;

	previosPivot = GetRandomRoadX();
	nextPivot = GetRandomRoadX();

	currentIntervalDirection = GetNextPivotLocation() - GetPreviosPivotLocation();
	currentIntervalDirection.Normalize();

	previousIntervalDirection = FVector2D(0, 1);

	StartDistanceMeasuring();
}
