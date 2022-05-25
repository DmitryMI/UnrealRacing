// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Wall.generated.h"

UCLASS()
class UNREALRACING_API AWall : public AObstacle
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	FVector2D segmentSize = FVector2D(80, 50);	

	UPROPERTY(EditDefaultsOnly, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperSprite> SourceSprite;

	UBoxComponent* boxCollider;
	TArray<UPaperSpriteComponent*> spriteComponents;

public:	
	// Sets default values for this actor's properties
	AWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastCreateHorizontalWall(int segments);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CreateHorizontalWall(int segments);

};
