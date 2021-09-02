// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "BasePawn.h"
#include "TankPawn.generated.h"



class USpringArmComponent;
class UCameraComponent;
class ATankPlayerController;


UCLASS()
class TANKOGEDDON_API ATankPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float MoveSpeed = 100.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float MovementSmootheness = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float RotationSpeed = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float RotationSmootheness = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    float TurretRotationSpeed = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
    TArray<FVector> PatrollingPoints;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
    float MovementAccuracy = 50.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void TargetDestroyed(AActor* Target) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void MoveForward(float AxisValue);

    UFUNCTION()
    void RotateRight(float AxisValue);

    UFUNCTION()
    const TArray<FVector>& GetPatrollingPoints() 
    { 
        return PatrollingPoints; 
    };
    
    UFUNCTION()
    float GetMovementAccurency() 
    { 
        return MovementAccuracy; 
    };

    UFUNCTION()
    FVector GetTurretForwardVector();

    UFUNCTION()
    void RotateTurretTo(FVector TargetPosition);

    UFUNCTION()
    FVector GetEyesPosition();

private:
    float TargetForwardAxisValue = 0.f;
    float CurrentForwardAxisValue = 0.f;
    float TargetRightAxisValue = 0.f;
    float CurrentRightAxisValue = 0.f;

    int32 AccumulatedScores = 0;

    UPROPERTY()
    ATankPlayerController* TankController;
};
