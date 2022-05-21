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
}

void ACarPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACarPlayerController::OnMouseXChanged(float value)
{
	int viewportSizeX;
	int viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);

	cursorVirtualPosition.X += value * mouseVirtualSensitivity;

	cursorVirtualPosition.X = FMath::Clamp(cursorVirtualPosition.X, 0.0f, (float)viewportSizeX);
}

void ACarPlayerController::OnMouseYChanged(float value)
{
	int viewportSizeX;
	int viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);

	cursorVirtualPosition.Y += value * mouseVirtualSensitivity;

	cursorVirtualPosition.Y = FMath::Clamp(cursorVirtualPosition.Y, 0.0f, (float)viewportSizeY);
}

void ACarPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	ACar* car = Cast<ACar>(pawn);
	possessedCar = car;
}

void ACarPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	possessedCar = nullptr;
}

bool ACarPlayerController::GetCursorPosition(FVector2D& position)
{
	if (!bUseVirtualCursor)
	{
		bool mouseValid = GetMousePosition(position.X, position.Y);

		if (!mouseValid)
		{
			//bUseVirtualCursor = true;
			//UE_LOG(LogTemp, Error, TEXT("[ACarPlayerController] No mouse!"));
			return false;
		}
		return true;
	}
	else
	{
		position = cursorVirtualPosition;
		return true;
	}
}

void ACarPlayerController::SendWaypoint()
{
	FVector2D cursorPos;
	bool cursorValid = GetCursorPosition(cursorPos);
	float mouseX = cursorPos.X;
	float mouseY = cursorPos.Y;

	FVector worldPosition;
	FVector worldDirection;
	DeprojectScreenPositionToWorld(mouseX, mouseY, worldPosition, worldDirection);

	worldDirection *= 10000;

	FPlane plane = FPlane(FVector(0, 0, 0), FVector(0, 1, 0));
	float intersectionT;
	FVector intersection;
	UKismetMathLibrary::LinePlaneIntersection(worldPosition, worldPosition + worldDirection, plane, intersectionT, intersection);

	//DrawDebugLine(GetWorld(), worldPosition, intersection, FColor::Green);

	DrawDebugBox(GetWorld(), intersection, FVector(50, 50, 50), FColor::Green);

	FVector2D waypoint = FVector2D(intersection.X, intersection.Z);
	possessedCar->SetWaypoint(waypoint);
}

void ACarPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		APawn* pawn = GetPawn();

		if (possessedCar == nullptr || possessedCar != pawn)
		{
			ACar* car = Cast<ACar>(pawn);
			possessedCar = car;
			return;
		}

		if (possessedCar != nullptr)
		{
			SendWaypoint();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[ACarPlayerController] Car not possessed!"));
		}
	}

}

bool ACarPlayerController::InputKey(const FInputKeyParams& Params)
{
	return Super::InputKey(Params);
}

void ACarPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BindInput();
}
