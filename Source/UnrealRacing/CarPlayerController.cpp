// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

void ACarPlayerController::BindInput()
{
	InputComponent->BindAxis(TEXT("MouseVertical"), this, &ACarPlayerController::OnMouseXChanged);
	InputComponent->BindAxis(TEXT("MouseHorizontal"), this, &ACarPlayerController::OnMouseYChanged);
}

void ACarPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (InputComponent != nullptr)
	{
		BindInput();
	}
}

void ACarPlayerController::OnMouseXChanged(float value)
{
}

void ACarPlayerController::OnMouseYChanged(float value)
{
}

void ACarPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	ACar* car = Cast<ACar>(pawn);
	posessedCar = car;
}

void ACarPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	posessedCar = nullptr;
}

void ACarPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	float mouseX;
	float mouseY;
	bool mouseValid = GetMousePosition(mouseX, mouseY);

	FVector worldPosition;
	FVector worldDirection;
	DeprojectScreenPositionToWorld(mouseX, mouseY, worldPosition, worldDirection);

	worldDirection *= 10000;

	DrawDebugLine(GetWorld(), worldPosition, worldPosition + worldDirection, FColor::Red);

	FPlane plane = FPlane(FVector(0, 0, 0), FVector(0, 1, 0));
	float intersectionT;
	FVector intersection;
	UKismetMathLibrary::LinePlaneIntersection(worldPosition, worldPosition + worldDirection, plane, intersectionT, intersection);

	//DrawDebugLine(GetWorld(), worldPosition, intersection, FColor::Green);

	DrawDebugBox(GetWorld(), intersection, FVector(50, 50, 50), FColor::Green);
}
