// Fill out your copyright notice in the Description page of Project Settings.


#include "MosherBase.h"


#include <activation.h>


#include "PhysXInterfaceWrapperCore.h"

// Sets default values
AMosherBase::AMosherBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}




// Called when the game starts or when spawned
void AMosherBase::BeginPlay()
{
	Super::BeginPlay();

	//setup static mesh
	TArray<UStaticMeshComponent*> staticMeshComps;
	GetComponents<UStaticMeshComponent>(staticMeshComps);
	statMeshComp = staticMeshComps[0];

	//setup state machine
	TArray<UMosherStateMachine*> stateMachines;
	GetComponents<UMosherStateMachine>(stateMachines);
	moshStateMachine = stateMachines[0];


	//setup sphere collider

	
	TArray<USphereComponent*> sphereComps;
	GetComponents<USphereComponent>(sphereComps);
	sphereCollider = sphereComps[0];


	//get ptr to inital sstate struct
	//moshStateMachine->UpdateStateStruct();

	_moshTimer	= 2.0f;

	//create collider obj
	
	
	
}



// Called every frame
void AMosherBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateNeighbourHood();

	if(_moshTimer <= 0)
	{

		mosherStruct = moshStateMachine->UpdateState(_neighbourhood,simulationManager,
        simulationManager->musicIntensity, aggression, musicFeel, groupThink);
		UpdateColour();
	}

	
	MoshBehavior(DeltaTime);


}

//handles general moshing behavior
void AMosherBase::MoshBehavior(float dT)
{
	FVector accel = FVector::ZeroVector;

		
	_moshTimer -= dT;
	//timer is for changing direction randomly
	if(_moshTimer <= 0)
	{
		accel += NormalizeSteeringForce(CalculateGaseousForce()
			*mosherStruct.GasLikeForce)*mosherStruct.GasLikeWeight;
		
		_moshTimer = simulationManager->moshRedirectTimer;
	}

	
	accel += NormalizeSteeringForce(CalculateForceToGlobalMiddle()
		*mosherStruct.ToGlobalMiddleForce)*mosherStruct.ToGlobalMiddleWeight;
	accel += NormalizeSteeringForce(CalculateForceFromGlobalMiddle()
		*mosherStruct.FromGlobalMiddleForce)*mosherStruct.FromGlobalMiddleWeight;
		
	accel += NormalizeSteeringForce(CalculateForceToLocalMiddle()
		*mosherStruct.ToLocalMiddleForce)*mosherStruct.ToLocalMiddleWeight;
	accel += NormalizeSteeringForce(CalculateForceFromLocalMiddle()
		*mosherStruct.FromLocalMiddleForce)*mosherStruct.FromLocalMiddleWeight;
	
	
	accel += NormalizeSteeringForce(CalculateVortexForce()
		*mosherStruct.VortexForce)*mosherStruct.VortexWeight;

	accel += NormalizeSteeringForce(CalculateForceToNeighbour()
		*mosherStruct.ThrashForce)*mosherStruct.ThrashWeight;

	
	if(simulationManager->DebugLineFinalVector)
		MoshDebugVector(accel*10.0f,FColor::Silver);
	
	statMeshComp->AddForce(accel*accelModifier*100000000.f*dT);

	mosherVelocity = statMeshComp->GetPhysicsLinearVelocity();
	
}


//FORCES//
//force that directs moshers randomly - gaslike
FVector AMosherBase::CalculateGaseousForce()
{
	FVector newVec;

	newVec = FMath::VRand();
	newVec.Z = 0;

	if(simulationManager->DebugLineGaseous)
		MoshDebugVector(newVec*10.0f,FColor::Red);
	return newVec;
}

//force towards closest neighbour
FVector AMosherBase::CalculateForceToNeighbour()
{
	FVector outVec;

	if(!_closestNeighbour)
		return outVec;
	
	outVec = _closestNeighbour->GetActorLocation() - GetActorLocation();
	outVec.Normalize();
	
	return outVec;
}


//force that directs moshers around the middle
FVector AMosherBase::CalculateVortexForce()
{

	FVector newVec;

	newVec = FVector::CrossProduct(FVector::UpVector,CalculateForceFromGlobalMiddle());
	newVec.Normalize();

	if(simulationManager->DebugLineVortex)
		MoshDebugVector(newVec,FColor::Blue);

	return newVec;
		
	

}

//force that directs moshers towards middle
FVector AMosherBase::CalculateForceToGlobalMiddle()
{
	FVector newVec;

	newVec = FVector(0.f,0.f,0.f) - this->GetActorLocation();
	newVec /= 100.0f;
	newVec.Normalize();
	//MoshDebugArrow(this->GetActorLocation(),newVec,FColor::Cyan);
	return newVec;
}

FVector AMosherBase::CalculateForceFromGlobalMiddle()
{
	FVector newVec;
	newVec = GetActorLocation() - FVector(0.f,0.f,0.f);
	newVec /= 100.f;
	newVec.Normalize();
	return newVec;
}


//force directing mosher to middle of their local neighbourhood
FVector AMosherBase::CalculateForceToLocalMiddle()
{
	FVector newVec;
	newVec = _neighbourhoodMiddle - this->GetActorLocation();
	newVec /= 100.0f;
	newVec.Normalize();
	//MoshDebugArrow(this->GetActorLocation(),newVec,FColor::Red);
	return newVec;
}

FVector AMosherBase::CalculateForceFromLocalMiddle()
{
	FVector newVec;
	newVec = GetActorLocation() - _neighbourhoodMiddle;
	newVec /= 100.0f;
	newVec.Normalize();
	return newVec;
}



//UTILITIES//

FVector AMosherBase::NormalizeSteeringForce(FVector inVec)
{
	FVector vecCopy = inVec;
	inVec.Normalize();
	return inVec * FMath::Clamp(vecCopy.Size(),0.0f,simulationManager->maxSteeringForce);
}


//Updates list of actors in neighbourhood and closest actor
void AMosherBase::UpdateNeighbourHood()
{
	_neighbourhood.Empty();
	
	sphereCollider->GetOverlappingActors(_neighbourhood,AMosherBase::StaticClass());

	float closestDist = 999.0f;
	FVector middlePoint = FVector::ZeroVector;

	_closestNeighbour = nullptr;
	
	for(auto actor : _neighbourhood)
	{
		if(actor == this)
			continue;
		
		// find closest neighbour
		FVector actorLocation = actor->GetActorLocation();
		float distance = (GetActorLocation() - actorLocation).Size();
		
		if(distance < closestDist)
		{
			closestDist = distance;
			_closestNeighbour = actor;
		}

		//find neighbourhood midle point
		middlePoint += actorLocation;
		middlePoint /= _neighbourhood.Num();
	}

	if(simulationManager->DebugShowNeighbourHoods)
		MoshSphere(middlePoint,FColor::Red);	
}

EMosherState AMosherBase::GetMosherState()
{
	return moshStateMachine->mosherState;
}


void AMosherBase::ResetMoshTimer()
{
	_moshTimer = simulationManager->moshRedirectTimer;
}


void AMosherBase::SetTraits(float aggro, float music, float group)
{
	aggression = aggro;
	musicFeel = music;
	groupThink = group;
}


void AMosherBase::UpdateColour()
{
	
	switch (moshStateMachine->mosherState)
	{
		case EMosherState::Moshing:
			if(statMeshComp->GetMaterial(0) != simulationManager->MoshMaterial)
					statMeshComp->SetMaterial(0,simulationManager->MoshMaterial);
			break;

		case EMosherState::Thrashing:
			if(statMeshComp->GetMaterial(0) != simulationManager->ThrashMaterial)
				statMeshComp->SetMaterial(0,simulationManager->ThrashMaterial);
			break;

		case EMosherState::Vortexing:
			if(statMeshComp->GetMaterial(0) != simulationManager->VortexMaterial)
				statMeshComp->SetMaterial(0,simulationManager->VortexMaterial);
			break;

		default:
			break;
	}
}



//DEBUG//

//wrapper for debug line


void AMosherBase::MoshDebugVector(FVector dir, FColor color)
{
	DrawDebugLine(GetWorld(),GetActorLocation() + FVector::ZAxisVector*25.f
		,GetActorLocation() + dir*100.0f,color,false, .01,9,
		2.5f);
}


void AMosherBase::MoshSphere(FVector centre,FColor color)
{
	DrawDebugSphere(GetWorld(),centre,5.0f,12,color,false,0.05,0,5.0f);
}


