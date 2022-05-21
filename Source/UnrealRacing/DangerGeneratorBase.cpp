// Fill out your copyright notice in the Description page of Project Settings.


#include "DangerGeneratorBase.h"
#include "UnrealRacingGameModeBase.h"

float ADangerGeneratorBase::GetTravelledDistance()
{
	return gameState->GetTravelledDistance();
}

// Sets default values
ADangerGeneratorBase::ADangerGeneratorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
}

// Called when the game starts or when spawned
void ADangerGeneratorBase::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		// Danger gerenerators should never exist on Clients
		Destroy();
		return;
	}

	gameMode = GetWorld()->GetAuthGameMode<AUnrealRacingGameModeBase>();
	if (gameMode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Mode is not of type AUnrealRacingGameModeBase!"))
	}
	else
	{
		gameState = gameMode->GetGameState<AUnrealRacingGameState>();
	}
}

// Called every frame
void ADangerGeneratorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADangerGeneratorBase::ActivateGenerator()
{
	bIsActive = true;
}

void ADangerGeneratorBase::DeactivateGenerator()
{
	bIsActive = false;
}

bool ADangerGeneratorBase::IsGeneratorActive()
{
	return bIsActive;
}

bool ADangerGeneratorBase::CanDeactivateNow()
{
	return true;
}

