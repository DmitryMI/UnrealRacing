// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DangerGeneratorBase.h"
#include "Obstacle.h"
#include "UnrealRacingGameModeBase.h"
#include "RandomDangerGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API ARandomDangerGenerator : public ADangerGeneratorBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	float obstaclesPerLineMinFactor = 0.01;

	UPROPERTY(EditDefaultsOnly)
	float obstaclesPerLineMaxFactor = 0.05;

	UPROPERTY(EditDefaultsOnly)
	float distanceBetweenLinesMin = 100;

	UPROPERTY(EditDefaultsOnly)
	float distanceBetweenLinesMax = 300;

	UPROPERTY(EditDefaultsOnly)
	float obstacleWidth = 50;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AObstacle>> ObstacleTypes;

	float lastLineDistance = 0;

	TSubclassOf<AObstacle> GetRandomObstacleType();

	int GetObstaclesPerLine();

	float GetDistanceBetweenLines();

	float GetDistanceSinceLastLine();

	void SpawnLineOfObstacles();

protected:
	virtual void Tick(float deltaTime) override;
	
public:
	virtual void ActivateGenerator() override;

	virtual void DeactivateGenerator() override;

	virtual bool CanDeactivateNow() override;
};
