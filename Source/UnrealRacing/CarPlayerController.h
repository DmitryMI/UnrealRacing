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
	ACar* possessedCar;

	UPROPERTY(EditDefaultsOnly)
	bool bUseVirtualCursor;

	UPROPERTY(EditAnywhere)
	float mouseVirtualSensitivity;

	UPROPERTY(EditAnywhere)
	FVector2D cursorVirtualPosition;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void BindInput();	

	virtual void OnMouseXChanged(float value);
	virtual void OnMouseYChanged(float value);

	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	bool GetCursorPosition(FVector2D& position);

	void SendWaypoint();
	virtual void Tick(float deltaTime) override;

	virtual bool InputKey(const FInputKeyParams& Params) override;

	virtual void SetupInputComponent() override;

	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};
