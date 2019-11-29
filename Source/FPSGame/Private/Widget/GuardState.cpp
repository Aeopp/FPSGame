// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardState.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "ConstructorHelpers.h"


UGuardState::UGuardState(const FObjectInitializer & ObjectInitializer)
	:Super(ObjectInitializer)
{
	static auto SetFont = ConstructorHelpers::FObjectFinder<UObject>(TEXT("Font'/Engine/EngineFonts/Roboto.Roboto'"));
	if (SetFont.Succeeded())
	{
		Font = SetFont.Object;
	}
	StateText = CreateDefaultSubobject<UTextBlock>(TEXT("StateText"));
	this->Canvas= CreateDefaultSubobject<UCanvasPanel>(TEXT("Canvas"));

	UE_LOG(LogTemp, Error, TEXT("UGuardState::UGuardState(const FObjectInitializer & ObjectInitializer)") );

	FSlateFontInfo FontInfo(Font, 60, FName(TEXT("Regular")));
	StateText->SetFont(FontInfo);

	FString Temp = TEXT("?");
	StateText->SetText(FText::FromString(Temp));

	FAnchors Anchors(0.5, 0.5, 0.5, 0.5);

	if (auto CanvasSlot = Cast<UCanvasPanelSlot>(StateText->GetParent()))
	{
		CanvasSlot->SetAnchors(Anchors);
		CanvasSlot->SetPosition(FVector2D(-20.f, -85.f));
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetSize(FVector2D(100.f, 30.f));
	}
	FSlateColor TextColor
	(FLinearColor(0.755, 0.51, 1.0f, 1.0f));

	FLinearColor ShadowColor(0, 0, 0, 0.4f);

	/*FSlateFontInfo FontInfo;
	FontInfo.FontObject = Font;
	FontInfo.TypefaceFontName = FName(TEXT("Regular"));
	FontInfo.Size = 60;*/

	StateText->SetColorAndOpacity(TextColor);

	StateText->SetJustification(ETextJustify::Center);
	StateText->SetShadowColorAndOpacity(ShadowColor);
	this->SetStateText(FText::FromString(TEXT("NONE")));
}
void UGuardState::SetStateText(FText SetText)
{
	UE_LOG(LogTemp, Error, TEXT("void UGuardState::SetStateText(FText SetText) << %s >> "),*SetText.ToString());
	

	StateText->SetText(SetText);
}

