// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MousePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void Tick(float deltaSeconds) override;
};
