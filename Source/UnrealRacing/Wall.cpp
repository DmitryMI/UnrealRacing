// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

// Sets default values
AWall::AWall()
{

}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
}

void AWall::NetMulticastCreateHorizontalWall_Implementation(int segments)
{
	float wallHeight = segmentSize.Y;
	float wallWidth = segmentSize.X * segments;
	FVector2D spriteSize = SourceSprite.Get()->GetSourceSize();
	FVector spriteComponentScale = FVector(segmentSize.X / spriteSize.X, 1, segmentSize.Y / spriteSize.Y);

	for (int i = 0; i < segments; i++)
	{
		FString name = FString::Printf(TEXT("Sprite %d"), i);
		UPaperSpriteComponent* spriteComponent = NewObject<UPaperSpriteComponent>(this, FName(name));
		spriteComponent->CreationMethod = EComponentCreationMethod::Instance;
		spriteComponent->SetMobility(EComponentMobility::Type::Movable);
		spriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		spriteComponent->RegisterComponent();
		spriteComponent->SetSprite(SourceSprite);
		float x = i * segmentSize.X - wallWidth / 2.0f;
		FVector relativeLocation = FVector(x, 0, 0);
		spriteComponent->SetRelativeLocation(relativeLocation);
		spriteComponent->SetWorldScale3D(spriteComponentScale);
		spriteComponents.Add(spriteComponent);
	}

	boxCollider = FindComponentByClass<UBoxComponent>();
	if (boxCollider == nullptr)
	{
		boxCollider = NewObject<UBoxComponent>(this, TEXT("WallBoxCollision"));
		boxCollider->SetMobility(EComponentMobility::Type::Movable);
		boxCollider->CreationMethod = EComponentCreationMethod::Instance;
		boxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		boxCollider->RegisterComponent();
	}
	FVector boxExtent = FVector(wallWidth / 2.0f, 25, wallHeight / 2.0f);
	boxCollider->SetBoxExtent(boxExtent);
}

void AWall::CreateHorizontalWall(int segments)
{
	NetMulticastCreateHorizontalWall(segments);
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

