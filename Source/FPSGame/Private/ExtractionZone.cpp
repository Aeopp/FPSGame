
// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/Engine.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
// Sets default values
AExtractionZone::AExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	
	OverlapComp->bGenerateOverlapEvents = true;
	OverlapComp->SetCollisionEnabled((ECollisionEnabled::QueryOnly));
	OverlapComp->SetCollisionResponseToAllChannels((ECR_Ignore));
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent((FVector(200.f)));
	OverlapComp->SetHiddenInGame(false);
	
	RootComponent = OverlapComp;
	
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this,&AExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->DecalSize = FVector(200.f, 200.f, 200.f);
}

// Called when the game starts or when spawned
void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExtractionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
};
void AExtractionZone::HandleOverlap
(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)	
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
			FString::Printf(TEXT("HandleOverlap Call !! ")));
	UE_LOG(LogTemp, Error, TEXT("Overlapped Extration ZOne !! "));
	
	auto MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn==nullptr) return;

	bool bIsSuccess = MyPawn->bIsCarryingObjective;
	if (bIsSuccess == false)
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);

	
	if (auto GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode()) )
	{
		GM->CompleteMission(MyPawn,bIsSuccess);

		/*if(bIsSuccess ==false)
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);*/
	}
}

