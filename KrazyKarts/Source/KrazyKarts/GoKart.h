// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

/*
* Members *
*/

private:

	FVector Velocity;

	// Mass of car (kg)
	UPROPERTY(EditAnywhere)
	float Mass;

	// The force applied to the car at full throttle (N)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16.0f;

	UPROPERTY(EditAnywhere)
	float RollingResistance;

	// Min turning radius of car, full lock (m)
	UPROPERTY(EditAnywhere)
	float TurningRadius = 10.0f;


	float Throttle;

	float SterringThrow;
/*
* Methods *
*/
private:
	void ComputeForwardVelocity(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime);
	FVector GetAirResistanceForce();
	FVector GetRollingResistance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this pawn's properties
	AGoKart();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void MoveForward(float Value);

	void MoveRight(float Value);
};
