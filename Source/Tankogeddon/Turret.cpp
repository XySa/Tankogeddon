// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <GameFramework/Controller.h>
#include <Kismet/KismetMathLibrary.h>

#include "Tankogeddon.h"
#include "Cannon.h"
#include "HealthComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
    if (TurretMeshTemp)
    {
        TurretMesh->SetStaticMesh(TurretMeshTemp);
    }

    UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
    if (BodyMeshTemp)
    {
        BodyMesh->SetStaticMesh(BodyMeshTemp);
    }
}

int32 ATurret::GetScores() const
{
    return DestructionScores;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    FTimerHandle TargetingTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Targeting()
{
    if (IsPlayerInRange())
    {
        RotateToPlayer();
    }

    if (CanFire() && GetActiveCannon() && GetActiveCannon()->IsReadyToFire())
    {
        Fire();
    }
}

void ATurret::RotateToPlayer()
{
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
    FRotator CurrRotation = TurretMesh->GetComponentRotation();
    TargetRotation.Pitch = CurrRotation.Pitch;
    TargetRotation.Roll = CurrRotation.Roll;
    TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
    return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
    FVector TargetingDir = TurretMesh->GetForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    DirToPlayer.Normalize();
    float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
    return AimAngle <= Accurency;
}
