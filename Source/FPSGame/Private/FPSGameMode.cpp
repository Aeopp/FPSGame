// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.


#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Widget/MissionResult.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


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
void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bIsMissonCompleted)
{
	if(InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr); 
	}

	// 블루프린트 구현 이벤트로 하고싶다면 !!

	
	if (auto PC = Cast<APlayerController>(InstigatorPawn->GetController()))
	{
		TArray<AActor*> Cameras;

		if (auto AddViewPort = CreateWidget<UMissionResult>(PC,*MissionCompleteWidget))
		{
			AddViewPort->AddToViewport();
			AddViewPort->SetMissionResult(bIsMissonCompleted);
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

	this->OnMissionCompleted(InstigatorPawn,bIsMissonCompleted);
}
