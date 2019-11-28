// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"

#include "ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
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
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&AFPSAIGuard::OnSeePawnEvent);
	
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
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
		
	}
}

