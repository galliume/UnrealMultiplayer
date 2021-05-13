// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		SetReplicates(true);//sync between server and client
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) { //if code is running on the server or the client

		FVector Location = GetActorLocation();
		FVector GlobalTargetLocation = GetTransform().TransformPosition(m_Target);
		FVector Direction = (GlobalTargetLocation - Location).GetSafeNormal();

		Location += m_Speed * DeltaTime * Direction;

		SetActorLocation(Location);
	}	
}