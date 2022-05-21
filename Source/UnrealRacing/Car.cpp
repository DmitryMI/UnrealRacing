// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector2D location = GetLocation2D();

		FVector2D waypointDirection = waypoint - location;

		DrawDebugLine(GetWorld(), FVector(location.X, 100, location.Y), FVector(waypoint.X, 100, waypoint.Y), FColor::Green);

		float targetAngle = FMath::Atan2(waypointDirection.Y, waypointDirection.X);

		SetRotation2D(targetAngle);
	}
}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACar::SetWaypoint(const FVector2D& value)
{
	waypoint = value;
}

FVector2D ACar::GetLocation2D()
{
	FVector location = GetActorLocation();

	return FVector2D(location.X, location.Z);
}

void ACar::SetRotation2D(float angle)
{
	//FRotator rotation = FRotator(angle, 0, 0);
	FRotator rotation = FRotator(FMath::RadiansToDegrees(angle), 0, 0);
	//FQuat quat = FQuat(0, 1, 0, angle);
	SetActorRotation(rotation);
}

