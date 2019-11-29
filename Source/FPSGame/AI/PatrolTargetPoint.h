// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PatrolTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API APatrolTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	APatrolTargetPoint();
	UPROPERTY()
	class UBoxComponent* BoxComp;

	UFUNCTION()
	void OnComponentBeginOverlap
	(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
