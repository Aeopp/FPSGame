// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerController.h"
#include "ConstructorHelpers.h"
#include "Widget/MissionResult.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AFPSPlayerController::AFPSPlayerController()
{
	static auto SetWidget = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/UI/WBP_GameOver.WBP_GameOver_C'"));
	if (SetWidget.Succeeded())
		this->MissionResult = SetWidget.Class;
}

void AFPSPlayerController::ClientOnMissionCompleted_Implementation(APawn* InstigatorPawn, bool bIsSuccessed,
	TSubclassOf<AActor> ViewTarget, float BlendTime)
{
	UE_LOG(LogTemp, Warning, TEXT("	FPSPlayerController OnMissionCompleted Call !!  "));

	if (auto ResultWidget = CreateWidget<UMissionResult>(this, MissionResult))
	{
		GetPawn()->DisableInput(this);
		
		ResultWidget->AddToViewport();
		ResultWidget->SetMissionResult(bIsSuccessed);

		TArray<AActor*> Cameras;

		if (ViewTarget)
			UGameplayStatics::GetAllActorsOfClass(this, ViewTarget, OUT Cameras);

		if (Cameras.Num())
		{
			this->SetViewTargetWithBlend(Cameras[0], BlendTime,
				EViewTargetBlendFunction::VTBlend_Cubic
				, 0, false);
		}
	}
}