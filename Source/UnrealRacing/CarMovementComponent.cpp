// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/HUD.h"

void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* owner = GetOwner();
	ENetRole ownerRole = GetOwnerRole();
	ENetRole remoteRole = owner->GetRemoteRole();

	bool isClient = GetWorld()->GetNetMode() == NM_Client;

	if (ownerRole == ROLE_Authority || ownerRole == ROLE_AutonomousProxy)
	{
		FVector linearVelocity;
		float angleDeltaRadians;
		CalculateVelocities(DeltaTime, linearVelocity, angleDeltaRadians);
		
		float currentAngle = GetRotation2D();
		currentAngle += angleDeltaRadians;
		FQuat nextRotation = CalculateQuat2D(currentAngle);

		Velocity = linearVelocity / DeltaTime;
		UpdateComponentVelocity();

		FHitResult hitResult;
		SafeMoveUpdatedComponent(linearVelocity, nextRotation, true, hitResult);
		if (hitResult.Component != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Hit!"));
		}
	}
	else if (ownerRole == ROLE_SimulatedProxy)
	{
		// We are owned by remove player or remote server.
		if (networkSmoothingEnabled)
		{
			const FRepMovement& repMovement = GetOwner()->GetReplicatedMovement();			

			float replicationPeriod = 1.0f / GetOwner()->NetUpdateFrequency;

			if (replicationPeriod < DeltaTime)
			{
				replicationPeriod = DeltaTime;
			}

			float timeSinceReplication = (replicationPeriod - timeToNextReplication);
			float interpAlpha = timeSinceReplication / replicationPeriod;
			timeToNextReplication -= DeltaTime;

			interpAlpha = FMath::Clamp(interpAlpha, 0.0f, networkInterpolationAlphaThreshold);

			FVector targetLocation;
			if (networkExtrapolationEnabled)
			{
				targetLocation = netNewLocation + repMovement.LinearVelocity * replicationPeriod;
			}
			else
			{
				targetLocation = netNewLocation;
			}

			FVector currentLocation = UpdatedComponent->GetRelativeLocation();

			FVector locationInterp = FMath::Lerp(netOldLocation, targetLocation, interpAlpha);

			//DrawDebugBox(GetWorld(), locationInterp, FVector(50, 50, 50), FColor::Cyan);
			//DrawDebugBox(GetWorld(), targetLocation, FVector(50, 50, 50), FColor::Blue);

			FQuat currentRotation = UpdatedComponent->GetRelativeRotation().Quaternion();
			FQuat rotationInterp = FMath::Lerp(netOldRotation, netNewRotation, interpAlpha);

			if (timeSinceReplication < networkTimeSinceReplicationThreshold)
			{
				UpdatedComponent->SetRelativeLocation(locationInterp);
				UpdatedComponent->SetRelativeRotation(rotationInterp);
			}
			else
			{
				UpdatedComponent->SetRelativeLocation(netNewLocation);
				UpdatedComponent->SetRelativeRotation(netNewRotation);
			}
		}
	}
	else
	{
		// No role, actor is nullptr
	}
}

void UCarMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	gameState = GetWorld()->GetGameState<AUnrealRacingGameState>();
}

float UCarMovementComponent::GetRotation2D() const
{
	FQuat rotationQuat = UpdatedComponent->GetRelativeRotation().Quaternion();

	float currentAngleRad = rotationQuat.GetTwistAngle(FVector(0, -1, 0));
	return currentAngleRad;
}

FQuat UCarMovementComponent::CalculateQuat2D(float angleRad) const
{
	FQuat nextRotation = FQuat(FVector(0, -1, 0), angleRad);
	return nextRotation;
}

void UCarMovementComponent::CalculateVelocities(float deltaTime, FVector& linear, float& angularRad) const
{
	FVector location = UpdatedComponent->GetRelativeLocation();
	FVector waypoint3d = FVector(waypoint.X, 0, waypoint.Y);
	FVector waypointDirection = waypoint3d - location;
	waypointDirection.Y = 0;	

	float targetAngleRad;

	if (waypointDirection.SquaredLength() < 1.0f)
	{
		targetAngleRad = HALF_PI;

		linear = FVector::Zero();
	}
	else
	{
		float directionLength = waypointDirection.Length();
		float frameStepMax = maxLinearSpeed * deltaTime;
		float frameStep = FMath::Clamp(directionLength, 0.0f, frameStepMax);
		FVector directionNormal = waypointDirection / directionLength;

		FVector faceTarget = FVector(
			directionNormal.X * maxLinearSpeed,
			0,
			directionNormal.Z * maxLinearSpeed + gameState->GetRoadSpeed());

		DrawDebugLine(GetWorld(), location, location + faceTarget, FColor::Green);

		linear = directionNormal * frameStep;
		linear.Y = 0;

		targetAngleRad = FMath::Atan2(faceTarget.Z, faceTarget.X);
	}

	//DrawDebugLine(GetWorld(), location, waypoint3d, FColor::Green);
	//DrawDebugLine(GetWorld(), location, location + UpdatedComponent->GetForwardVector() * 100, FColor::Red);	

	float currentAngleRad = GetRotation2D();
	float rotationDeltaRad = FMath::FindDeltaAngleRadians(currentAngleRad, targetAngleRad);

	float frameAngularStepMax = FMath::DegreesToRadians(maxAngularSpeedDeg) * deltaTime;

	angularRad = FMath::Clamp(rotationDeltaRad, -frameAngularStepMax, frameAngularStepMax);

	//UE_LOG(LogTemp, Warning, TEXT("Current Angle: %3.2f, Target Angle: %3.2f, Delta: %3.2f, Velocity: %3.2f"), currentAngleDeg, targetAngleDeg, rotationDeltaDeg, angularDeg);
}

void UCarMovementComponent::SendClientAdjustment()
{
}

bool UCarMovementComponent::ForcePositionUpdate(float DeltaTime)
{
	return false;
}

void UCarMovementComponent::SmoothCorrection(const FVector& OldLocation, const FQuat& OldRotation, const FVector& NewLocation, const FQuat& NewRotation)
{
	netOldLocation = OldLocation;
	netNewLocation = NewLocation;
	netOldRotation = OldRotation;
	netNewRotation = NewRotation;	

	float repPeriod = 1.0f / GetOwner()->NetUpdateFrequency;

	//DrawDebugBox(GetWorld(), OldLocation, FVector(50, 50, 50), FColor::Red, false, repPeriod);
	//DrawDebugBox(GetWorld(), NewLocation, FVector(50, 50, 50), FColor::Green, false, repPeriod);

	timeToNextReplication = 1.0f / GetOwner()->NetUpdateFrequency;
}

FNetworkPredictionData_Client* UCarMovementComponent::GetPredictionData_Client() const
{
	return nullptr;
}

FNetworkPredictionData_Server* UCarMovementComponent::GetPredictionData_Server() const
{
	return nullptr;
}

void UCarMovementComponent::SetWaypoint(FVector2D value)
{
	waypoint = value;
}

UCarMovementComponent::UCarMovementComponent()
{
}

bool UCarMovementComponent::HasPredictionData_Client() const
{
	return false;
}

bool UCarMovementComponent::HasPredictionData_Server() const
{
	return false;
}

void UCarMovementComponent::ResetPredictionData_Client()
{
}

void UCarMovementComponent::ResetPredictionData_Server()
{
}
