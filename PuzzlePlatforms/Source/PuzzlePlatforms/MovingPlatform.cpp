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

	m_GlobalStartLocation = GetActorLocation();
	m_GlobalTargetLocation = GetTransform().TransformPosition(m_Target);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_ActiveTriggers > 0) {

		if (HasAuthority()) { //if code is running on the server or the client

			FVector Location = GetActorLocation();
			float Length = (m_GlobalTargetLocation - m_GlobalStartLocation).Size();
			float LengthTravelled = (Location - m_GlobalStartLocation).Size();

			if (LengthTravelled >= Length) {
				FVector Swap = m_GlobalStartLocation;
				m_GlobalStartLocation = m_GlobalTargetLocation;
				m_GlobalTargetLocation = Swap;
			}

			FVector Direction = (m_GlobalTargetLocation - m_GlobalStartLocation).GetSafeNormal();
			Location += m_Speed * DeltaTime * Direction;

			SetActorLocation(Location);
		}	
	}
}


void AMovingPlatform::AddActiveTrigger()
{
	m_ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (m_ActiveTriggers > 0) {
		m_ActiveTriggers--;
	}
}