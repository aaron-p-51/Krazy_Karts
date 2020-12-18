// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"

// Engine Includes
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mass = 1000.0f;
	MaxDrivingForce = 10000.0f;
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
	
	ProcessForwardMovement(DeltaTime);
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}

void AGoKart::ProcessForwardMovement(float DeltaTime)
{
	// Get the force moving the car forward. Throttle is bound between -1 and 1
	// therefore it scales MaxDrivingForce between full throttle forward and
	// full throttle backwards.
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	// Get Acceleration of car (F=m*a)
	FVector Acceleration = Force / Mass;

	// Using the current acceleration adjust the velocity. Acceleration in m/s^2 * s
	// gives m/s. Acceleration can be negative to give backwards movement (Throttle -1 to 1)
	Velocity = Velocity + Acceleration * DeltaTime;

	// Get the movement of the car this frame from the velocity computed.
	// Velocity is m/s * s gives us meters. Default Unreal units in
	// cm so value is multipled by 100.
	FVector Translation = Velocity * DeltaTime * 100.0f;

	// Apply offset to actor
	AddActorWorldOffset(Translation);
}

