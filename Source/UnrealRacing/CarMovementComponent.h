// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interfaces/NetworkPredictionInterface.h"
#include "UnrealRacingGameState.h"
#include "CarMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class UNREALRACING_API UCarMovementComponent : public UPawnMovementComponent, public INetworkPredictionInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	FVector2D waypoint;

	UPROPERTY(Transient)
	AUnrealRacingGameState* gameState;

	UPROPERTY(EditAnywhere)
	float maxAngularSpeedDeg = 360;

	UPROPERTY(EditAnywhere)
	float maxLinearSpeed = 800;

	UPROPERTY(EditAnywhere)
	bool networkSmoothingEnabled = true;

	UPROPERTY(EditAnywhere)
	bool networkExtrapolationEnabled = true;

	UPROPERTY(EditAnywhere)
	float networkInterpolationAlphaThreshold = 1.5f;

	UPROPERTY(EditAnywhere)
	float networkTimeSinceReplicationThreshold = 1.0f;

	FVector netOldLocation;
	FVector netNewLocation;
	FQuat netOldRotation;
	FQuat netNewRotation;
	float timeToNextReplication = 0;
	
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent
	(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	float GetRotation2D() const;
	FQuat CalculateQuat2D(float angleRad) const;

	void CalculateVelocities(float deltaTime, FVector& linear, float& angularRad) const;

	// INetworkPredictionInterface
	virtual void SendClientAdjustment() override;

	/** (Server) Trigger a position update on clients, if the server hasn't heard from them in a while. @return Whether movement is performed. */
	virtual bool ForcePositionUpdate(float DeltaTime) override;

	/** (Client) After receiving a network update of position, allow some custom smoothing, given the old transform before the correction and new transform from the update. */
	virtual void SmoothCorrection(const FVector& OldLocation, const FQuat& OldRotation, const FVector& NewLocation, const FQuat& NewRotation) override;

	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual class FNetworkPredictionData_Server* GetPredictionData_Server() const override;

	virtual bool HasPredictionData_Client() const override;

	virtual bool HasPredictionData_Server() const override;

	virtual void ResetPredictionData_Client()  override;

	virtual void ResetPredictionData_Server() override;
	

public:
	void SetWaypoint(FVector2D value);

	UCarMovementComponent();
};
