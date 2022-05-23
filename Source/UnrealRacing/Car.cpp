// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//movementComponent = CreateDefaultSubobject<UCarMovementComponent>("CarMovementComponent", true);


}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();

	movementComponent = FindComponentByClass<UCarMovementComponent>();
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsServer = GetWorld()->GetNetMode() != NM_Client;
	FString netMode = bIsServer ? "Server" : "Client";
	FString name;
	GetName(name);

	if (GetLocalRole() == ROLE_Authority || GetLocalRole() == ROLE_AutonomousProxy)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[ACar] (%s): Name == %s, Waypoint == (%3.2f, %3.2f)"), *netMode, *name, waypoint.X, waypoint.Y);
		
		movementComponent->SetWaypoint(waypoint);
	}
}

// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACar::ServerSetWaypoint_Implementation(const FVector2D& value)
{
	waypoint = value;
}

void ACar::SetWaypoint(const FVector2D& value)
{
	waypoint = value;
	ServerSetWaypoint(value);
}

FVector2D ACar::GetLocation2D()
{
	FVector location = GetActorLocation();

	return FVector2D(location.X, location.Z);
}

