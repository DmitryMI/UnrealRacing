// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "DangerGeneratorBase.h"
#include "ZigzagGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API AZigzagGenerator : public ADangerGeneratorBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float gateWidthMin = 300.0f;
	UPROPERTY(EditAnywhere)
	float gateWidthMax = 500.0f;

	UPROPERTY(EditAnywhere)
	float intervalLenghtMin = 500.0f;
	UPROPERTY(EditAnywhere)
	float intervalLenghtMax = 1000.0f;

	UPROPERTY(EditAnywhere)
	FVector2D obstacleSize = FVector2D(80, 50);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AObstacle> ObstacleType;

	float previosPivot;
	float nextPivot;

	float nextPivotDistance;
	float distanceToNextPivotBase;

	FVector2D currentIntervalDirection;
	FVector2D previousIntervalDirection;

	float GetPivotDistance();

	float GetPreviosPivotZ();
	float GetNextPivotZ();

	FVector2D GetPreviosPivotLocation();
	FVector2D GetNextPivotLocation();

	float GetRandomRoadX();

	float GetGateWidth();

	void SpawnSegment();

protected:
	virtual void Tick(float deltaTime) override;
	virtual void ActivateGeneratorInternal() override;
};
