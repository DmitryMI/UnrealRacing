// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UnrealRacingGameState.h"
#include "ObstacleMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API UObstacleMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(Transient)
	AUnrealRacingGameState* gameState;

	bool bDoRetryMovement;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent
	(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;

	bool ResolvePenetrationImpl(const FVector& ProposedAdjustment, const FHitResult& Hit, const FQuat& NewRotationQuat) override;

public:
	UObstacleMovementComponent();
};
