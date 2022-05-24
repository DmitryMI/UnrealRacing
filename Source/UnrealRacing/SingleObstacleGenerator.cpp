// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleObstacleGenerator.h"

void ASingleObstacleGenerator::ActivateGenerator()
{
	FVector2D roadCenter = gameState->GetRoadCenter();
	float roadZ = gameState->GetRoadTopZ();
	SpawnObstacle(roadCenter.X, roadZ, ObstacleType);
}
