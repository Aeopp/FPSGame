// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted,
};

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Comps)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;
	UFUNCTION()
	void OnRep_GuardState();
	
	FORCEINLINE void SetGuardState(EAIState AIState)
	{
		if (Role != ROLE_Authority)return;
		
		if (GuardState  == AIState)return;

		GuardState = AIState;
		OnRep_GuardState();
		// OnstateChanged(AIState);
		// OnRep_GuardState();
	     // OnstateChanged(AIState);
	}
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = AI)
	void OnstateChanged(EAIState NewState);
	virtual void OnstateChanged_Implementation(EAIState NewState);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget|State")
	class UWidgetComponent* StateWidget;

	UFUNCTION(Category=DelegateBind)
	void  OnSeePawnEvent(APawn*  Pawn);
	UFUNCTION(Category = DelegateBind)
	void OnHearNoiseDelegateEvent
	(APawn*  NoiseInstigator ,const FVector&  Location,float Volume);
public:
	
	FRotator LookRot;
	FTimerHandle RotReturnTimer;
	UFUNCTION()
	void RotReturnOrient();

	UFUNCTION()
	void MissionFail(APawn* const DetectedPawn);

	FRotator StartRotation;

	int32 CurrentPatrolIndex = 0;
	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<class ATargetPoint*> PatrolLocs;
	void PatrolMove();
protected:
	UPROPERTY(EditInstanceOnly, Category = AI)
	bool bIsPartol = false;

	FTimerHandle PatrolTimer;
	
	void PatrolStop();
};
