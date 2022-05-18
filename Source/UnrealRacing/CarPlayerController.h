// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Car.h"
#include "CarPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API ACarPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ACar* posessedCar;

protected:
	virtual void BeginPlay() override;

	virtual void BindInput();	

	virtual void OnMouseXChanged(float value);
	virtual void OnMouseYChanged(float value);

	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;
};
