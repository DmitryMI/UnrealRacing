// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealRacingGameState.h"
#include "DangerGeneratorBase.generated.h"

class AUnrealRacingGameModeBase;


UCLASS()
class UNREALRACING_API ADangerGeneratorBase : public AActor
{
	GENERATED_BODY()
	
private:	
	
protected:	

	UPROPERTY(EditAnywhere)
	bool bIsGeneratorEnabled = true;

	AUnrealRacingGameModeBase* gameMode;
	AUnrealRacingGameState* gameState;

	bool bIsActive = false;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	template<typename T>
	T ScaleWithDifficultyLinear(const T& from, const T& to)
	{
		float scale = gameState->GetTravelledDistance() / gameState->GetMaxDifficultyDistance();
		return (to - from) * scale + from;
	}

	float GetTravelledDistance();

	AActor* SpawnObstacle(float x, float y, TSubclassOf<AActor> obstacleType);
	
public:
	// Sets default values for this actor's properties
	ADangerGeneratorBase();

	UFUNCTION(BlueprintCallable)
	virtual void ActivateGenerator();
	UFUNCTION(BlueprintCallable)
	virtual void DeactivateGenerator();

	UFUNCTION(BlueprintCallable)
	bool IsGeneratorActive();

	UFUNCTION(BlueprintCallable)
	bool IsGeneratorEnabled();

	UFUNCTION(BlueprintCallable)
	void SetGeneratorEnabled(bool isEnabled);

	UFUNCTION(BlueprintCallable)
	virtual bool CanDeactivateNow();
};
