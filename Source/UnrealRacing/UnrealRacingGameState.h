// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealRacingGameState.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRACING_API AUnrealRacingGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	// Replicated

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = UpdateMaterialsRoadSpeed)
	float roadSpeed = 300;

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = RoadSize_OnRep)
	FVector2D roadSize = FVector2D(1000, 2000);

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = RoadCenter_OnRep)
	FVector2D roadCenter = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = IsRaceStarted_OnRep)
	bool bIsRaceStarted = false;


	// Not replicated
	float travelledDistance = 0;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* roadMaterial;
	UMaterialInstanceDynamic* roadMaterialInstanceDynamic;

	UPROPERTY(EditDefaultsOnly)
	int roadAnimatorsRows = 3;

	UPROPERTY(EditDefaultsOnly)
	int roadAnimatorsCols = 3;

	TArray<TArray<AActor*>> roadAnimatorsMatrix;



	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> roadAnimatorType;

	const float roadTileSize = 1024;

	UPROPERTY(EditDefaultsOnly)
	float maximumDifficultyDistance = 153000;

	UPROPERTY(EditDefaultsOnly)
	float roadTopZ = 1000;	

	void SetupRoadAnimators();

	UFUNCTION()
	void UpdateMaterialsRoadSpeed();


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float deltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void IsRaceStarted_OnRep();

	UFUNCTION()
	void RoadSize_OnRep();

	UFUNCTION()
	void RoadCenter_OnRep();

public:
	AUnrealRacingGameState();

	UFUNCTION(BlueprintCallable)
	float GetRoadSpeed();

	UFUNCTION(BlueprintCallable)
	float GetRoadTopZ();

	UFUNCTION(BlueprintCallable)
	float GetTravelledDistance();

	UFUNCTION(BlueprintCallable)
	float GetMaxDifficultyDistance();

	UFUNCTION(BlueprintCallable)
	void SetRoadSize(FVector2D size);

	UFUNCTION(BlueprintCallable)
	FVector2D GetRoadSize();

	UFUNCTION(BlueprintCallable)
	void SetRoadCenter(FVector2D center);

	UFUNCTION(BlueprintCallable)
	FVector2D GetRoadCenter();

	UFUNCTION(BlueprintCallable)
	void StartRace();

	UFUNCTION(BlueprintCallable)
	bool IsRaceStarted();
};
