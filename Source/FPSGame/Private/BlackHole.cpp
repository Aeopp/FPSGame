// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SMMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMMeshComp"));
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	WarpZone   = CreateDefaultSubobject<USphereComponent>(TEXT("WarpZone"));
	SphereComp->SetSphereRadius(BlackHoleRadius);
	SphereComp->bGenerateOverlapEvents = true;
	RootComponent = SphereComp;
	SMMeshComp->SetupAttachment(RootComponent);
	SMMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WarpZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WarpZone->SetSphereRadius(WarpZoneRadius);
	WarpZone->bGenerateOverlapEvents = true;
	
	WarpZone->OnComponentBeginOverlap.
	AddDynamic(this,&ABlackHole::OnActorBeginOverlapEvent);
	WarpZone->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<UPrimitiveComponent*> OutComps;
	GetOverlappingComponents(OUT OutComps);
	if (OutComps.Num())
	{
		for (auto Target : OutComps)
		{
			if (Target->IsSimulatingPhysics())
			{
				float Radius = SphereComp->GetScaledSphereRadius();
				Target->AddRadialForce(GetActorLocation(), Radius, -2000.f, ERadialImpulseFalloff::RIF_Constant,true);
			}
		}
	}
}

void ABlackHole::OnActorBeginOverlapEvent(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}

}

