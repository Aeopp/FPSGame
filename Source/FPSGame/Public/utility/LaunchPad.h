// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();
	virtual void BeginPlay()override;
protected:
public:	
	UFUNCTION()
	void LaunchTarget(AActor*OverlappedActor, AActor* OtherActor);
private:
	UPROPERTY(VisibleInstanceOnly, Category = Comp)
	class UBoxComponent *BoxComp;

	UPROPERTY(EditAnywhere, Category = Comp)
	class UStaticMeshComponent* STMeshComp;

	UPROPERTY()
	class UParticleSystem* ParticleSystem;
	
	UPROPERTY()
	TArray<AActor*> Targets;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Valilaible, meta = (AllowprivateAccess = true))
		bool bIsZOverride = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Valilaible, meta = (AllowprivateAccess = true))
		bool bIsXYOverride = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Valilaible, meta = (AllowprivateAccess = true))
	FVector LaunchForce = FVector(0, 0, 3000000.f);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Valilaible, meta = (AllowprivateAccess = true))
		float ImpulseForce = 1000.f;
	
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category=Comp, meta = (AllowprivateAccess = true))
	class UArrowComponent* LaunchArrowComp;
};
