// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionResult.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UMissionResult : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget),BlueprintReadWrite,EditAnywhere)
	class UCanvasPanel* Canvas;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UTextBlock* RenderMissionResult;

	void SetMissionResult(bool bIsMissionResult);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsMissionComplete = false;
};
