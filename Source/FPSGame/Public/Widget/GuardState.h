// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuardState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API UGuardState : public UUserWidget
{
	GENERATED_BODY()
public:
	UGuardState(const FObjectInitializer & ObjectInitializer);
	UPROPERTY(meta=(BindWidget),BlueprintReadWrite,EditAnywhere)
	class UTextBlock* StateText;
	UPROPERTY(meta = (BindWidget),BlueprintReadWrite, EditAnywhere)
	class UCanvasPanel* Canvas;

	UPROPERTY()
	UObject* Font;
	
	UFUNCTION(BlueprintCallable,Category=TextBind)
	void SetStateText(FText SetText);

};
