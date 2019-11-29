// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolTargetPoint.h"
#include "Enemy/FPSAIGuard.h"
#include "Components/BoxComponent.h"

APatrolTargetPoint::APatrolTargetPoint():Super()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->bGenerateOverlapEvents = true;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComp->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetRelativeLocation(FVector::ZeroVector);
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&APatrolTargetPoint::OnComponentBeginOverlap);
}

void APatrolTargetPoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("OnComponentBeginOverlap Call PatrolMove "));

	if (auto AIGuard = Cast<AFPSAIGuard>(OtherActor))
	{
		AIGuard->PatrolMove();
		UE_LOG(LogTemp, Error, TEXT("OnComponentBeginOverlap Call PatrolMove "));
	}
}

