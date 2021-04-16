// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "MosherStateMachine.h"
#include "DrawDebugHelpers.h"
#include "MosherStateMachine.h"
#include "SimManager.h"
#include "Chaos/AABBTree.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MosherBase.generated.h"





//Base Class for the mosher; handles movement/physics
UCLASS(Blueprintable)
class CROWDSIM_API AMosherBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMosherBase();

	//Dictates how likely agent is to start thrashing
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mosher Traits")
	float aggression;

	//Dictates to what an extent this agent is affected by the music
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mosher Traits")
	float musicFeel;

	//Dictates how likely agent is to take on state of its neighbours
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Mosher Traits")
	float groupThink;

	UPROPERTY(EditAnywhere,Category="Mosher Debug")
	bool ShowDebugNeighbourhood;


	
	//simulation Manager holds all sim constants etc.
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ASimManager* simulationManager;
	
	UPROPERTY(VisibleAnywhere)
	FVector mosherVelocity;
	
	UPROPERTY(EditAnywhere)
	float accelModifier;
	

	UPROPERTY(EditAnywhere)
	UMosherStateMachine* moshStateMachine;

	FMosherStruct mosherStruct;

private:
	UStaticMeshComponent* statMeshComp;
	USphereComponent* sphereCollider;

	//NEIGHBOURHOOD
	TArray<AActor*> _neighbourhood;

	FVector _neighbourhoodMiddle;
	AActor* _closestNeighbour;
	
	UPROPERTY(VisibleAnywhere)
	float _moshTimer;
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoshBehavior(float);



private:


	
	//utilities
	FVector NormalizeSteeringForce(FVector);
	void UpdateNeighbourHood();
	
	//functions for calculating forces
	FVector CalculateGaseousForce();
	FVector	CalculateVortexForce();
	FVector CalculateForceToNeighbour();
	FVector CalculateForceToGlobalMiddle();
	FVector CalculateForceFromGlobalMiddle();
	FVector CalculateForceToLocalMiddle();
	FVector CalculateForceFromLocalMiddle();

	//Debug functions
	void MoshDebugVector(FVector,FColor);
	void MoshSphere(FVector,FColor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetMoshTimer();

	UFUNCTION(BlueprintCallable)
	void SetTraits(float aggro,float music,float group);

	void UpdateColour();

	EMosherState GetMosherState();
	

};




