// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealRacingGameState.h"
#include "Net/UnrealNetwork.h"


AUnrealRacingGameState::AUnrealRacingGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUnrealRacingGameState::SetupRoadAnimators()
{
	if (roadMaterial == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("roadMaterial is nullptr!"));
		return;
	}

	if (roadAnimatorType.Get() == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("roadAnimatorType is undefined!"));
		return;
	}

	roadMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(roadMaterial, this);

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), roadAnimatorType, actors);

	for (int i = 0; i < roadAnimatorsRows; i++)
	{
		roadAnimatorsMatrix.Add(TArray<AActor*>());
		for (int j = 0; j < roadAnimatorsCols; j++)
		{
			roadAnimatorsMatrix[i].Add(nullptr);
		}
	}

	for (int i = 0; i < roadAnimatorsRows; i++)
	{
		for (int j = 0; j < roadAnimatorsCols; j++)
		{
			int index = i * roadAnimatorsCols + j;
			AActor* actor = actors[index];

			UPaperSpriteComponent* sprite = Cast<UPaperSpriteComponent>(actor->GetComponentByClass(UPaperSpriteComponent::StaticClass()));
			sprite->SetMaterial(0, roadMaterialInstanceDynamic);

			float x = roadTileSize * j - roadTileSize * (int)(roadAnimatorsCols / 2);
			float z = roadTileSize * i - roadTileSize * (int)(roadAnimatorsRows / 2);

			FVector location = FVector(x, 0, z);
			actor->SetActorLocation(location);

			roadAnimatorsMatrix[i][j] = actor;
		}
	}


	UpdateMaterialsRoadSpeed();
}

float AUnrealRacingGameState::GetRoadSpeed()
{
	return roadSpeed;
}

float AUnrealRacingGameState::GetRoadTopZ()
{
	return roadTopZ;
}

void AUnrealRacingGameState::UpdateMaterialsRoadSpeed()
{
	if (roadMaterialInstanceDynamic != nullptr)
	{
		//roadMaterialInstanceDynamic->SetScalarParameterValue("RoadSpeed", -roadSpeed / roadTileSize);
		roadMaterialInstanceDynamic->SetScalarParameterValue("RoadSpeed", 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateMaterialsRoadSpeed: roadMaterialInstanceDynamic is nullptr"))
	}
}

float AUnrealRacingGameState::GetTravelledDistance()
{
	return travelledDistance;
}

float AUnrealRacingGameState::GetMaxDifficultyDistance()
{
	return maximumDifficultyDistance;
}

void AUnrealRacingGameState::SetRoadSize(FVector2D size)
{
	if (!HasAuthority())
	{
		return;
	}
	roadSize = size;
}

void AUnrealRacingGameState::SetRoadCenter(FVector2D center)
{
	if (!HasAuthority())
	{
		return;
	}
	roadCenter = center;
}

FVector2D AUnrealRacingGameState::GetRoadSize()
{
	return roadSize;
}


FVector2D AUnrealRacingGameState::GetRoadCenter()
{
	return FVector2D::Zero();
}

void AUnrealRacingGameState::StartRace()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Client called StartRace()"));
		return;
	}

	bIsRaceStarted = true;
}

bool AUnrealRacingGameState::IsRaceStarted()
{
	return bIsRaceStarted;
}


void AUnrealRacingGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay()"))
	SetupRoadAnimators();
}

void AUnrealRacingGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AUnrealRacingGameState::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HasAuthority())
	{
		travelledDistance += roadSpeed * deltaTime;
		//roadSpeed += deltaTime;
	}

	float minAnimatorZ = -roadTileSize * 1.5;
	float maxAnimatorZ = roadTileSize * 1.5;

	for (int i = 0; i < roadAnimatorsRows; i++)
	{
		FVector location = roadAnimatorsMatrix[i][0]->GetActorLocation();
		float rowZ = location.Z - roadSpeed * deltaTime;
		
		float diff = location.Z - minAnimatorZ;
		if (diff < 0)
		{
			int topRow = (i + 2) % roadAnimatorsRows;
			FVector topRowLocation = roadAnimatorsMatrix[topRow][0]->GetActorLocation();
			rowZ = topRowLocation.Z + roadTileSize;
			if (topRow >= i)
			{
				rowZ -= roadSpeed * deltaTime;
			}
		}

		for (int j = 0; j < roadAnimatorsCols; j++)
		{			
			AActor* animator = roadAnimatorsMatrix[i][j];
			FVector animatorLocation = animator->GetActorLocation();
			animatorLocation.Z = rowZ;
			animator->SetActorLocation(animatorLocation);
		}
	}
}

void AUnrealRacingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnrealRacingGameState, roadSpeed);
	DOREPLIFETIME(AUnrealRacingGameState, roadCenter);
	DOREPLIFETIME(AUnrealRacingGameState, roadSize);
	DOREPLIFETIME(AUnrealRacingGameState, bIsRaceStarted);
}

void AUnrealRacingGameState::IsRaceStarted_OnRep()
{
}

void AUnrealRacingGameState::RoadSize_OnRep()
{
}

void AUnrealRacingGameState::RoadCenter_OnRep()
{
}
