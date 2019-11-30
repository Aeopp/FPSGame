// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();
	void CompleteMission(APawn* InstigatorPawn,bool bIsMissionCompleted);
	//UFUNCTION(BlueprintImplementableEvent, Category = GameMode)
	//void OnMissionCompleted(APawn* InstigatorPawn, bool bIsMissionCompleted);

	UPROPERTY(EditAnywhere, Category = "BP|Widget")
	TSubclassOf<class UMissionResult>MissionCompleteWidget;
	UPROPERTY(EditDefaultsOnly, Category = "View|Target")
	TSubclassOf<AActor> ViewTarget;
	UPROPERTY(EditAnywhere, Category = "View|BlendTime")
	float ViewChangeBlendTime = 3.f;
};



