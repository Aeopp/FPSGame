// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere,Category=Comp)
	class UStaticMeshComponent* SMMeshComp;
	UPROPERTY(EditAnywhere, Category = Comp)
	class USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere, Category = Comp)
	class USphereComponent* WarpZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubClass)
	TSubclassOf<class UStaticMesh>SMMesh;
	float BlackHoleRadius = 3000.f;
	float WarpZoneRadius = 100.f;

	UFUNCTION()
	void OnActorBeginOverlapEvent
	(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult & SweepResult);
};
