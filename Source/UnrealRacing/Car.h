// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "CarMovementComponent.h"
#include "Car.generated.h"

UCLASS()
class UNREALRACING_API ACar : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FVector2D waypoint;

	UPROPERTY(Transient)
	UCarMovementComponent* movementComponent;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:	
	// Sets default values for this pawn's properties
	ACar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	virtual void ServerSetWaypoint(const FVector2D& value);

	UFUNCTION(BlueprintCallable)
	virtual void SetWaypoint(const FVector2D& value);

	UFUNCTION(BlueprintCallable)
	FVector2D GetLocation2D();

};
