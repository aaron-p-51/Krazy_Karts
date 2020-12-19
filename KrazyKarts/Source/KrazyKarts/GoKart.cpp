// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"

// Engine Includes
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

static const int METERS_TO_CENTIMETERS = 100;

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mass = 1000.0f;
	MaxDrivingForce = 10000.0f;
	RollingResistance = 0.015f;
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ComputeForwardVelocity(DeltaTime);
	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTime);	
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}

void AGoKart::MoveRight(float Value)
{
	SterringThrow = Value;
}

void AGoKart::ComputeForwardVelocity(float DeltaTime)
{
	// Get the force moving the car forward. Throttle is bound between -1 and 1
	// therefore it scales MaxDrivingForce between full throttle forward and
	// full throttle backwards.
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	// add the air and rolling resistance to the force applied from throttle
	Force += GetAirResistanceForce();
	Force += GetRollingResistance();

	// Get Acceleration of car (F=m*a)
	FVector Acceleration = Force / Mass;

	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;

	// Using the current acceleration adjust the velocity. Acceleration in m/s^2 * s
	// gives m/s. Acceleration can be negative to give backwards movement (Throttle -1 to 1)
	Velocity = Velocity + Acceleration * DeltaTime;
}

FVector AGoKart::GetAirResistanceForce()
{
	return -Velocity.SizeSquared() * Velocity.GetSafeNormal() * DragCoefficient;
}

FVector AGoKart::GetRollingResistance()
{
	float NormalForceMagnitude = Mass * (-GetWorld()->GetGravityZ() / 100);
	return -Velocity.GetSafeNormal() * RollingResistance * NormalForceMagnitude;
}

void AGoKart::UpdateLocationFromVelocity(float DeltaTime)
{
	// Compute distance to move from velocity m/s by multiplying by deltaTime to give meters.
	// Unreal units in cm so multiply by 100.0f;
	FVector Translation = Velocity * METERS_TO_CENTIMETERS * DeltaTime;

	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}

void AGoKart::ApplyRotation(float DeltaTime)
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / TurningRadius * SterringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}
