// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MissionResult.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
void UMissionResult::SetMissionResult(bool bIsMissionResult)
{
	FString Result = bIsMissionResult ? TEXT("Mission Success !! ") : TEXT("Mission Fail !! ");
	RenderMissionResult->SetText(FText::FromString(Result));
	auto CanvasSlot = Cast<UCanvasPanelSlot>
		(RenderMissionResult->GetParent());
	if (CanvasSlot == nullptr)return;
	
	FSlateFontInfo Info;
	Info.Size = 40.f;
	RenderMissionResult->SetFont(Info);
	RenderMissionResult->SetJustification(ETextJustify::Center);
	FAnchors Anchor(0.5f, 0.5f, 0.5f, 0.5f);
	CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	CanvasSlot->SetPosition(FVector2D(34.f, 3.f));

	UE_LOG(LogTemp, Error, TEXT("GOOD!"));
}

