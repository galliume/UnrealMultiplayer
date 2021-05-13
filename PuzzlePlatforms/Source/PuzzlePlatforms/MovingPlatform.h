#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float m_Speed = 20;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector m_Target;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:
	FVector m_GlobalTargetLocation;
	FVector m_GlobalStartLocation;

	UPROPERTY(EditAnywhere)
	int m_ActiveTriggers = 1;
};
