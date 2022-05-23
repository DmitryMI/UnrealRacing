// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "CarMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
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

	//UCharacterMovementComponent* characterMovement;

	UPROPERTY(Replicated, ReplicatedUsing = Health_OnRep)
	float health = 100.0f;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Health_OnRep();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
