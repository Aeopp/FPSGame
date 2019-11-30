// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	SetReplicateMovement(true);
	SetReplicates(true);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	SphereComp= CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
}
// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffects();
};

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation
	(this, PickupFX, GetActorLocation());
}
void AFPSObjectiveActor::TempFunc_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("void AFPSObjectiveActor::TempFunc_Implementation()"));
}
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	if(Role == ROLE_Authority    )
	{
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->bIsCarryingObjective = true;

			this->Destroy();
		}
	}
}





