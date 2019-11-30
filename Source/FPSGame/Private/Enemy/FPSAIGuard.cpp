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
#include "Components/WidgetComponent.h"
#include "Widget/GuardState.h"
#include "AI/PatrolTargetPoint.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "UnrealNetwork.h"
#include <utility>


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
	this->SetReplicates(true);
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->StateWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp")); 
	this->PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	static auto SetSKMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Content/Characters/DwarfGrunt/SkelMesh/DwarfGrunt_R_new.DwarfGrunt_R_new'"));
	static auto SetAnimSeq = ConstructorHelpers::FObjectFinder<UAnimSequence>(TEXT("AnimSequence'/Game/Content/Characters/DwarfGrunt/Anims/Idle_1_new.Idle_1_new'"));
	static auto SetStateWidget = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/UI/WBP_GuardState.WBP_GuardState_C'"));
	
	if(SetStateWidget.Succeeded() )
	{
		StateWidget->SetupAttachment(RootComponent);
		StateWidget->SetWidgetClass(SetStateWidget.Class);
		StateWidget->SetWidgetSpace(EWidgetSpace::World);
		StateWidget->SetDrawAtDesiredSize(true);
		StateWidget->SetRelativeLocation(FVector(0,0,110.f)); 
		StateWidget->Activate(true);
	}
	
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

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	StateWidget->SetRelativeLocation(FVector(0, 0, 110.f));
	LookRot = StartRotation = GetActorRotation();

	if (bIsPartol && PatrolLocs.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("void AFPSAIGuard::BeginPlay()Patrol Start"));
		this->PatrolMove();
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRot = GetActorRotation(); 
	
	if(LookRot != CurrentRot)
	{
		SetActorRotation(FMath::RInterpTo(CurrentRot, LookRot, DeltaTime, 1.5f));
	}
}

// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnstateChanged(this->GuardState);
}

void AFPSAIGuard::OnSeePawnEvent(APawn* Pawn)
{
	if(Pawn)
	{	
		DrawDebugLine(GetWorld(), GetActorLocation(), 
			Pawn->GetActorLocation(),
			FColor::Blue, false, 3.f);
		MissionFail(Pawn);
		SetGuardState(EAIState::Alerted);

		FVector Direction = Pawn->GetActorLocation() - GetActorLocation();
		Direction = Direction.GetSafeNormal(KINDA_SMALL_NUMBER);

		LookRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		LookRot.Pitch = 0.f;
		LookRot.Roll = 0.f;
		
		GetController()->StopMovement();
		/*if (GetMovementComponent()->IsActive() == false)return;
		GetMovementComponent()->SetActive(false);*/
		GetWorld()->GetTimerManager().SetTimer(PatrolTimer, this, &AFPSAIGuard::PatrolStop, 1.5f, false);
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
		
		LookRot = FRotationMatrix::MakeFromX(Direction).Rotator();
		LookRot.Pitch = 0.f;
		LookRot.Roll = 0.f;
		
		GetWorld()->GetTimerManager().ClearTimer(RotReturnTimer);
		GetWorld()->GetTimerManager().SetTimer(RotReturnTimer, this, &AFPSAIGuard::RotReturnOrient, 3.f, false);
		// MissionFail(NoiseInstigator);
		if (GuardState != EAIState::Alerted)  SetGuardState(EAIState::Suspicious);

		GetController()->StopMovement();
		/*if (GetMovementComponent()->IsActive() == false)return;
		GetMovementComponent()->SetActive(false);*/
		GetWorld()->GetTimerManager().SetTimer(PatrolTimer, this, &AFPSAIGuard::PatrolStop, 1.5f, false);
	}
}

void AFPSAIGuard::RotReturnOrient()
{
	SetGuardState(EAIState::Idle); 
	LookRot = StartRotation; 
	// SetActorRotation(this->OrientRotation);
}

void AFPSAIGuard::MissionFail(APawn* const DetectedPawn)
{
	AFPSGameMode *  GM = Cast<AFPSGameMode >(GetWorld()->GetAuthGameMode());
	
	if (DetectedPawn && GM ) 
	{
		GM->CompleteMission(DetectedPawn, false);	
	}
}

void AFPSAIGuard::PatrolMove()
{
	if ( PatrolLocs.Num()==false || bIsPartol==false )return;

	if (CurrentPatrolIndex >= PatrolLocs.Num()) CurrentPatrolIndex = 0;

	UNavigationSystem::SimpleMoveToActor(GetController(),PatrolLocs[CurrentPatrolIndex]);
	CurrentPatrolIndex++;
}

void AFPSAIGuard::PatrolStop()
{
	UE_LOG(LogTemp, Error, TEXT(""));
	 // GetMovementComponent()->SetActive(true);
}

void AFPSAIGuard::OnstateChanged_Implementation(EAIState NewState)
{
	UE_LOG(LogTemp, Error, TEXT("Call"));

	if (   auto GuardStateWidget = Cast<UGuardState>(StateWidget->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Error, TEXT("Success"));

		FString StateStr = TEXT("");

		switch (NewState)
		{
		case EAIState::Suspicious:
			StateStr = TEXT("?");
			break;
		case EAIState::Alerted:
			StateStr = TEXT("!");
			break;

		case EAIState::Idle:
			break;
		default:
			break;
		}

		GuardStateWidget->SetStateText(FText::FromString(StateStr));
	}
}
