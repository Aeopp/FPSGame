// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Widget/MissionResult.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "FPSGameState.h"
#include "FPSPlayerController.h"
AFPSGameMode::AFPSGameMode()
{
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static auto SetWidget = ConstructorHelpers::FClassFinder<UMissionResult>
	(TEXT("WidgetBlueprint'/Game/UI/WBP_GameOver.WBP_GameOver_C'"));
	static auto SetViewTargetActor = ConstructorHelpers::FClassFinder<AActor>
	(TEXT("Blueprint'/Game/Blueprints/BP_SpectatingViewpoint.BP_SpectatingViewpoint_C'"));

	if(SetWidget.Succeeded())
	{
		MissionCompleteWidget = SetWidget.Class;
	}

	if(SetViewTargetActor.Succeeded())
	{
		ViewTarget = SetViewTargetActor.Class;
	}
	
	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bIsMissionCompleted)
{
	bool bServer = (Role == ROLE_Authority);
	FString StrSerCli = bServer ? TEXT("Server") : TEXT("Client ");
	UE_LOG(LogTemp, Error, TEXT("GM Complete Mission Call Here is %s"), *StrSerCli);

	UE_LOG(LogTemp, Error, TEXT(" "));

	if (Role != ROLE_Authority)return;
	
	for ( auto PCIt = GetWorld()->GetPlayerControllerIterator(); PCIt;++PCIt)
	{
		 if ( auto Player = PCIt->Get()->GetPawn())
		 {
			  // Player->DisableInput(PCIt->Get());
		 	
			 //auto AddWidget = CreateWidget<UMissionResult>(PCIt->Get(), MissionCompleteWidget);
			 //AddWidget->AddToViewport();
			 //AddWidget->SetMissionResult(bIsMissionCompleted);
			if (  auto FPSPC =  Cast<AFPSPlayerController >(PCIt->Get()))
			{
				FPSPC->ClientOnMissionCompleted(Player, bIsMissionCompleted, ViewTarget, ViewChangeBlendTime);

			}
		 	
		 }
	}
	//if (auto ResultWidget = CreateWidget<UMissionResult>(this, MissionResult))
	//{
	//	ResultWidget->AddToViewport();
	//	ResultWidget->SetMissionResult(bIsSuccessed);

	//	TArray<AActor*> Cameras;

	//	if (ViewTarget)
	//		UGameplayStatics::GetAllActorsOfClass(this, ViewTarget, OUT Cameras);

	//	if (Cameras.Num())
	//	{
	//		this->SetViewTargetWithBlend(Cameras[0], BlendTime,
	//			EViewTargetBlendFunction::VTBlend_Cubic
	//			, 0, false);
	//	}
	//}
	/*if (InstigatorPawn)
	{
		if( auto bPC = Cast<APlayerController >(InstigatorPawn->GetController()))
		{
			InstigatorPawn->DisableInput(bPC);
		}
	}*/

	// 블루프린트 구현 이벤트로 하고싶다면 !!

	
	/*if (AFPSGameState * GS = GetGameState<AFPSGameState>())
	{
		GS->NetMulticastOnMissionCompleted(InstigatorPawn, bIsMissionCompleted,ViewTarget,this->ViewChangeBlendTime);
	}*/
}

