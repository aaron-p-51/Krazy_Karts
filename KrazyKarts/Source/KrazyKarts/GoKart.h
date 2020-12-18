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

	float Throttle;
/*
* Methods *
*/
public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void ProcessForwardMovement(float DeltaTime);

};
