// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Comps)
	class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION(Category=DelegateBind)
	void  OnSeePawnEvent(APawn*  Pawn);
	UFUNCTION(Category = DelegateBind)
	void OnHearNoiseDelegateEvent(
	APawn*  NoiseInstigator ,const FVector&  Location,float Volume);

	private:
	FTimerHandle RotReturnTimer;
	UFUNCTION()
	void RotReturnOrient();

	UFUNCTION()
	void MissionFail(APawn* const DetectedPawn);

	FRotator OrientRotation;
};
