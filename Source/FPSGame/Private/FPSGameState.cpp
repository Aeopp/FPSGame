// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "Engine/World.h"
#include "FPSPlayerController.h"
#include "GameFramework/PlayerController.h"

void AFPSGameState::NetMulticastOnMissionCompleted_Implementation(APawn* InstigatorPawn, bool bIsSuccessed, TSubclassOf<class AActor>ViewTarget, float BlendTime)
{
	/*UE_LOG(LogTemp, Warning, TEXT("void AFPSGameState::NetMulticastOnMissionCompleted_Implementation(APawn* InstigatorPawn, bool bIsSuccessed) Call  "));
	
	for (auto PC = GetWorld()->GetPlayerControllerIterator();  PC; ++PC    )
	{
		UE_LOG(LogTemp, Warning, TEXT("	for (auto PC = GetWorld()->GetPlayerControllerIterator();  PC; ++PC    )"));

		if (auto FPSPC = Cast<AFPSPlayerController>(PC->Get()))
		{
			if (FPSPC->IsLocalPlayerController())
			{

				UE_LOG(LogTemp, Warning, TEXT("	Yes FPSPC IS Locally PlayerController !! "));

				FPSPC->OnMissionCompleted(InstigatorPawn, bIsSuccessed,ViewTarget,BlendTime);

				if (auto Pawn = FPSPC->GetPawn())
				{
					Pawn->DisableInput(FPSPC);
					UE_LOG(LogTemp, Warning, TEXT("					Pawn->DisableInput(FPSPC);"));

				}
			}
	
		}
	}*/
}
