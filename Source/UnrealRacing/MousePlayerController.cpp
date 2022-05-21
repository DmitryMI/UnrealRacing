// Fill out your copyright notice in the Description page of Project Settings.


#include "MousePlayerController.h"

void AMousePlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HasAuthority())
	{
		APawn* pawn = GetPawn();

		if (HasAuthority())
		{
			float mouseX, mouseY;
			bool mouseValid = GetMousePosition(mouseX, mouseY);
			if (mouseValid)
			{
				if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
				{
					UE_LOG(LogTemp, Warning, TEXT("[AMousePlayerController] Client's authority. Mouse: (%3.2f, %3.2f)"), mouseX, mouseY);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[AMousePlayerController] Server's authority. Mouse: (%3.2f, %3.2f)"), mouseX, mouseY);
				}
			}
			else
			{
				if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
				{
					UE_LOG(LogTemp, Error, TEXT("[AMousePlayerController] Client's authority. Mouse invalid!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[AMousePlayerController] Server's authority. Mouse invalid!"));
				}
			}
		}
	}
}