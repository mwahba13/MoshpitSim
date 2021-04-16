// Fill out your copyright notice in the Description page of Project Settings.


#include "SimManager.h"

#include "MosherBase.h"

// Sets default values
ASimManager::ASimManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASimManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASimManager::SetTraitVectors(FVector2D aggro, FVector2D music, FVector2D group)
{
	aggression = aggro;
	musicFeel = music;
	groupThink = group;
}


float ASimManager::GenerateGaussianFloat(float x, float std)
{
	float u1 = 1.0 - FMath::FRand();
	float u2 = 1.0 - FMath::FRand();

	float randStdNormal = FMath::Sqrt(-2.0 * FMath::Loge(u1))
	* FMath::Sin(2.0 *3.14592*u2);
	return FMath::Clamp(x+std*randStdNormal,0.0f,100.0f);
}



FMosherStruct* ASimManager::GetStateStructPtr(EMosherState inState)
{
	switch (inState)
	{
		case EMosherState::Idle:
			return  &IdleStateStruct;
			break;
		case EMosherState::Moshing:
			return &MoshStateStruct;
			break;
		case EMosherState::Vortexing:
			return &VortexStateStruct;
			break;
	case EMosherState::Thrashing:
		return &ThrashingStateStruct;
		break;
		default:
			return &IdleStateStruct;
			break;
	}
}

FMosherStruct ASimManager::GetStateStruct(EMosherState inState)
{
	switch (inState)
	{
	case EMosherState::Idle:
		return  IdleStateStruct;
		break;
	case EMosherState::Moshing:
		return MoshStateStruct;
		break;
	case EMosherState::Vortexing:
		return VortexStateStruct;
		break;
	case EMosherState::Thrashing:
		return ThrashingStateStruct;
		break;
	default:
		return IdleStateStruct;
		break;
	}
}

