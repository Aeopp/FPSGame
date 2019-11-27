// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidget.h"
#include "GameFramework/PlayerController.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static auto SetWidget = ConstructorHelpers::FClassFinder<UUserWidget>
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
}
void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)
{
	if(InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr); 
	}
	
// 	this->OnMissionCompleted(InstigatorPawn);
	
	if (auto PC = Cast<APlayerController>(InstigatorPawn->GetController()))
	{
		TArray<AActor*> Cameras;

		if (auto AddViewPort = CreateWidget<UUserWidget>(PC,*MissionCompleteWidget))
		{
			AddViewPort->AddToViewport();
		}
		
		if (ViewTarget)
		UGameplayStatics::GetAllActorsOfClass(this,ViewTarget, OUT Cameras);

		if (Cameras.Num())
		{
			PC->SetViewTargetWithBlend(Cameras[0], this->ViewChangeBlendTime,
			EViewTargetBlendFunction::VTBlend_Cubic
			, 0, false);
		}
	}
}