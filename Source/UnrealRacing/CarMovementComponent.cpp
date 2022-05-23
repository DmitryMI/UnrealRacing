// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/HUD.h"

void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
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

		Velocity = linearVelocity;
		UpdateComponentVelocity();
		FHitResult hitResult;
		SafeMoveUpdatedComponent(linearVelocity, nextRotation, true, hitResult);
	}
	else if (ownerRole == ROLE_SimulatedProxy)
	{
		// We are owned by remove player or remote server.
	}
	else
	{
		// No role, actor is nullptr
	}
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
		targetAngleRad = FMath::Atan2(waypointDirection.Z, waypointDirection.X);

		float directionLength = waypointDirection.Length();
		float frameStepMax = maxLinearSpeed * deltaTime;
		float frameStep = FMath::Clamp(directionLength, 0.0f, frameStepMax);
		FVector directionNormal = waypointDirection / directionLength;

		linear = directionNormal * frameStep;
		linear.Y = 0;
	}

	DrawDebugLine(GetWorld(), location, waypoint3d, FColor::Green);
	DrawDebugLine(GetWorld(), location, location + UpdatedComponent->GetForwardVector() * 100, FColor::Red);	

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
	/*
	FVector locationInterp = FMath::Lerp(OldLocation, NewLocation, 0.1f);
	FQuat rotationInterp = FMath::Lerp(OldRotation, NewRotation, 0.1f);

	UpdatedComponent->SetRelativeLocation(locationInterp);
	UpdatedComponent->SetRelativeRotation(rotationInterp);
	*/
}

void UCarMovementComponent::SetWaypoint(FVector2D value)
{
	waypoint = value;
}

UCarMovementComponent::UCarMovementComponent()
{
}
