// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DangerGeneratorBase.h"
#include "Obstacle.h"
#include "Wall.h"
#include "WallGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API AWallGenerator : public ADangerGeneratorBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float gateWidthMin = 100;

	UPROPERTY(EditDefaultsOnly)
	float gateWidthMax = 500;

	UPROPERTY(EditDefaultsOnly)
	float distanceBetweenLinesMin = 100;

	UPROPERTY(EditDefaultsOnly)
	float distanceBetweenLinesMax = 300;

	UPROPERTY(EditDefaultsOnly)
	float obstacleWidth = 90;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWall> WallType;

	float lastLineDistance = 0;

	int GetGateWidthInSlots();

	int GetObstaclesPerLine();

	float GetDistanceBetweenLines();

	float GetDistanceSinceLastLine();

	void SpawnWallOfObstacles();

protected:
	virtual void Tick(float deltaTime) override;

	virtual void ActivateGeneratorInternal() override;

public:
	virtual void DeactivateGenerator() override;

	virtual bool CanDeactivateNow() override;
};
