// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DangerGeneratorBase.h"
#include "SingleObstacleGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API ASingleObstacleGenerator : public ADangerGeneratorBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ObstacleType;
public:
	virtual void ActivateGenerator();
};
