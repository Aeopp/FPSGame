// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "DrawDebugHelpers.h"
#include "ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	static auto SetSKMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Content/Characters/DwarfGrunt/SkelMesh/DwarfGrunt_R_new.DwarfGrunt_R_new'"));
	static auto SetAnimSeq = ConstructorHelpers::FObjectFinder<UAnimSequence>(TEXT("AnimSequence'/Game/Content/Characters/DwarfGrunt/Anims/Idle_1_new.Idle_1_new'"));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	GetCapsuleComponent()->InitCapsuleSize(45.f,45.f);
	if(SetSKMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SetSKMesh.Object);
		if(SetAnimSeq.Succeeded())
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->SetAnimation(SetAnimSeq.Object);
			GetMesh()->PlayAnimation(SetAnimSeq.Object, true);
		}
	}
	
	PawnSensingComp->HearingThreshold = 820.f;
	PawnSensingComp->LOSHearingThreshold = 1500.f;
	PawnSensingComp->SightRadius = 3100.f;
	PawnSensingComp->SetPeripheralVisionAngle(32.f);
	
	PawnSensingComp->OnSeePawn.AddDynamic
	(this,&AFPSAIGuard::OnSeePawnEvent);
	PawnSensingComp->OnHearNoise.AddDynamic
	(this, &AFPSAIGuard::OnHearNoiseDelegateEvent);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OrientRotation = GetActorRotation();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AFPSAIGuard::OnSeePawnEvent(APawn* Pawn)
{
	if(Pawn)
	{	
		DrawDebugLine(GetWorld(), GetActorLocation(), 
			Pawn->GetActorLocation(),
			FColor::Blue, false, 3.f);
		MissionFail(Pawn);
	}
}
void AFPSAIGuard::OnHearNoiseDelegateEvent
(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if(NoiseInstigator)
	{
		DrawDebugSphere(GetWorld(), 
		Location,
		30.f, 12, FColor::Yellow, 
		false, 3.f);

		FVector Direction =  Location - GetActorLocation(); 
		Direction = Direction.GetSafeNormal(KINDA_SMALL_NUMBER);
		
		FRotator DirRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		DirRot.Pitch = 0.f;
		DirRot.Roll = 0.f;
		SetActorRotation(DirRot);

		GetWorld()->GetTimerManager().ClearTimer(RotReturnTimer);
		GetWorld()->GetTimerManager().SetTimer(RotReturnTimer, this, &AFPSAIGuard::RotReturnOrient, 3.f, false);
		// MissionFail(NoiseInstigator);
	}
}
void AFPSAIGuard::RotReturnOrient()
{
	SetActorRotation(this->OrientRotation);
}

void AFPSAIGuard::MissionFail(APawn* const DetectedPawn)
{
	AFPSGameMode *  GM = Cast<AFPSGameMode >(GetWorld()->GetAuthGameMode());
	
	if (DetectedPawn && GM ) 
	{
		GM->CompleteMission(DetectedPawn, false);	
	}
}

