// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleMovementComponent.h"
#include "Car.h"

void UObstacleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	gameState = GetWorld()->GetGameState<AUnrealRacingGameState>();
}

UObstacleMovementComponent::UObstacleMovementComponent()
{
	PlaneConstraintNormal = FVector(0, 1, 0);
	PlaneConstraintOrigin = FVector(0, 5.0f, 0);
	bConstrainToPlane = true;
}

void UObstacleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector location = UpdatedComponent->GetRelativeLocation();

	//UE_LOG(LogTemp, Warning, TEXT("Obstacle Location Start: (%3.2f, %3.2f, %3.2f)"), location.X, location.Y, location.Z);

	Velocity = FVector(0, 0, -gameState->GetRoadSpeed());
	FVector Delta = Velocity * DeltaTime;	

	FHitResult hitResult;

	FVector oldLocation = UpdatedComponent->GetRelativeLocation();

	bool bMoved = false;
	while (!bMoved)
	{
		bMoved = SafeMoveUpdatedComponent(Delta, FQuat::Identity, true, hitResult);

		if (hitResult.IsValidBlockingHit())
		{
			UE_LOG(LogTemp, Error, TEXT("Obstacle Movement Hit!"));
			HandleImpact(hitResult, DeltaTime, Delta);
			bMoved = false;

			// Reset movement, we need all-or-nothing
			UpdatedComponent->SetRelativeLocation(location, false);
		}
	}

	FVector nextLocation = UpdatedComponent->GetRelativeLocation();
	FVector realDelta = nextLocation - oldLocation;
	if (realDelta != Delta)
	{
		UE_LOG(LogTemp, Error, TEXT("Obstacle Movement: wrong delta!"));
	}

	UpdateComponentVelocity();
}

bool UObstacleMovementComponent::ResolvePenetrationImpl(const FVector& ProposedAdjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	AActor* penetratedActor = Hit.GetActor();
	if (penetratedActor == nullptr)
	{
		return true;
	}

	APawn* penetratedPawn = Cast<APawn>(penetratedActor);	
	if (penetratedPawn == nullptr)
	{
		//return Super::ResolvePenetrationImpl(ProposedAdjustment, Hit, NewRotationQuat);
		return true;
	}

	UCarMovementComponent* carMovement = Cast<UCarMovementComponent>(penetratedPawn->GetMovementComponent());
	if (carMovement == nullptr)
	{
		return true;
		//return Super::ResolvePenetrationImpl(ProposedAdjustment, Hit, NewRotationQuat);
	}

	FVector adjustment = -ProposedAdjustment;

	bool bCarPushedAway = carMovement->OnObstacleImpact(this, Hit, 0.02f, adjustment);

	return bCarPushedAway;
}

void UObstacleMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	ACar* car = Cast<ACar>(Hit.GetActor());

	if (car == nullptr)
	{
		return;
	}

	// If we hit a car, continue movement and shift the car

	UCarMovementComponent* carMovement = Cast<UCarMovementComponent>(car->GetMovementComponent());
	if (carMovement != nullptr)
	{
		carMovement->OnObstacleImpact(this, Hit, TimeSlice, MoveDelta);
	}
}
