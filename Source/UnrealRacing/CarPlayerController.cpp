// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPlayerController.h"

void ACarPlayerController::BindInput()
{
	InputComponent->BindAxis(TEXT("MouseVertical"), this, &ACarPlayerController::OnMouseXChanged);
	InputComponent->BindAxis(TEXT("MouseHorizontal"), this, &ACarPlayerController::OnMouseYChanged);
}

void ACarPlayerController::BeginPlay()
{
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
