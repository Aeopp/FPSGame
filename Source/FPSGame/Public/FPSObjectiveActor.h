// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceClass.h"
#include "FPSObjectiveActor.generated.h"

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor , public IInterfaceClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,Category="Components")
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComp;
	void PlayEffects();
	UPROPERTY(EditDefaultsOnly,Category=Components)
	class UParticleSystem* PickupFX;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TempFunc();
	virtual void TempFunc_Implementation() override;
public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;
public:
};
