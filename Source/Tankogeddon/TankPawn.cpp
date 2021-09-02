// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Math/UnrealMathUtility.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>

#include "Tankogeddon.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include <Components/BoxComponent.h>
#include "Scorable.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
    SpringArm->SetupAttachment(BodyMesh);
    SpringArm->bDoCollisionTest = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float AxisValue)
{
    TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
    TargetRightAxisValue = AxisValue;
}

FVector ATankPawn::GetTurretForwardVector()
{
    return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
    FRotator CurrRotation = TurretMesh->GetComponentRotation();
    TargetRotation.Pitch = CurrRotation.Pitch;
    TargetRotation.Roll = CurrRotation.Roll;
    TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TurretRotationSpeed));
}

FVector ATankPawn::GetEyesPosition()
{
    return CannonSetupPoint->GetComponentLocation();
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
    TankController = Cast<ATankPlayerController>(GetController());
}

void ATankPawn::TargetDestroyed(AActor* Target)
{
    if (IScorable* Scorable = Cast<IScorable>(Target))
    {
        AccumulatedScores += Scorable->GetScores();
        UE_LOG(LogTankogeddon, Log, TEXT("Destroyed target %s. Current scores: %d"), *Target->GetName(), AccumulatedScores);
    }
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Tank movement
    CurrentForwardAxisValue = FMath::FInterpTo(CurrentForwardAxisValue, TargetForwardAxisValue, DeltaTime, MovementSmootheness);
    
    FVector CurrentLocation = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector MovePosition = CurrentLocation + ForwardVector * CurrentForwardAxisValue * MoveSpeed * DeltaTime;

    FHitResult* SweepHitResult = nullptr;
    SetActorLocation(MovePosition, true, SweepHitResult);
    if (SweepHitResult)
    {
        CurrentForwardAxisValue = 0.f;
    }

    // Tank rotation
    CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmootheness);

    UE_LOG(LogTankogeddon, Verbose, TEXT("CurrentRightAxisValue = %f TargetRightAxisValue = %f"), CurrentRightAxisValue, TargetRightAxisValue);

    FRotator CurrentRotation = GetActorRotation();
    float YawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
    YawRotation += CurrentRotation.Yaw;

    FRotator NewRotation = FRotator(0.f, YawRotation, 0.f);
    SetActorRotation(NewRotation);

    // Turret rotation
    if (TankController)
    {
        FVector MousePos = TankController->GetMousePos();
        RotateTurretTo(MousePos);
    }
}
