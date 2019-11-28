// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
// Sets default values
ALaunchPad::ALaunchPad()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	this->STMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STMeshComp"));
	this->LaunchArrowComp= CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	RootComponent = BoxComp;
	STMeshComp->SetupAttachment(RootComponent);
	STMeshComp->bGenerateOverlapEvents = false;
	BoxComp->bGenerateOverlapEvents = true;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxComp->InitBoxExtent(FVector(100.f, 100.f, 100.f));

	static auto SetParticle =  ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Effects/gold_ore_lost.gold_ore_lost'"));
	if (SetParticle.Succeeded())
	{
		ParticleSystem = SetParticle.Object;
		
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Particle COnstrucotr FInder Fail"));
	
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	this->OnActorBeginOverlap.AddDynamic(this, &ALaunchPad::LaunchTarget);
}


void ALaunchPad::LaunchTarget(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor&&OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, GetActorLocation());
		
		FString PrintActorsName = OverlappedActor->GetName() + OtherActor->GetName();
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, PrintActorsName, false);

		if (auto Character = Cast<ACharacter>(OtherActor))
			Character->LaunchCharacter(this->LaunchForce, this->bIsXYOverride,this->bIsZOverride);
		else
		{
			if ( auto OverlapComp = Cast<UPrimitiveComponent> ( OtherActor->GetComponentByClass(UPrimitiveComponent::StaticClass())) )
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *OverlapComp->GetFullName());
				
				OverlapComp->AddImpulse(LaunchArrowComp->GetForwardVector()*ImpulseForce,NAME_None,true);
			}
		}
	}
}

