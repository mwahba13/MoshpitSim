// Fill out your copyright notice in the Description page of Project Settings.


#include "MosherStateMachine.h"
#include "MosherBase.h"

#include <string>


#include "PhysXInterfaceWrapperCore.h"

// Sets default values for this component's properties
UMosherStateMachine::UMosherStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMosherStateMachine::BeginPlay()
{
	Super::BeginPlay();

	// init state
	//SimManager->GetStateStruct(mosherState,mosherStateStruct);

	
}

FMosherStruct UMosherStateMachine::UpdateState(TArray<AActor*> neighbours,ASimManager* SimManager,
	 float musicIntensity,
	float aggression, float musicFeel, float groupThink)
{

	//spontaneous transition from idle to moshing
	if(mosherState == EMosherState::Idle)
		StateTransition(EMosherState::Moshing);
	
	aggression /= 100.0f;
	musicFeel /= 100.0f;
	groupThink /= 100.0f;
	
	float thrashQuotient = (aggression+musicIntensity);
	
	float neighbourQuotient = (groupThink)+(musicFeel);
	
	float vortexQuotient = (musicIntensity)+(musicFeel);

	

	/*
	if(maxQuotient == thrashQuotient)
		StateTransition(EMosherState::Thrashing);
	else if(maxQuotient == neighbourQuotient)
		StateTransition(GetMostCommonState(neighbours));
	else if(maxQuotient == vortexQuotient)
		StateTransition(EMosherState::Vortexing);
*/
	FMosherStruct newStruct = SimManager->GetStateStruct(mosherState);


	//aggression
	newStruct.ThrashWeight = FMath::Clamp(newStruct.ThrashWeight + aggression,0.0f,1.0f);
	newStruct.FromGlobalMiddleWeight = FMath::Clamp(newStruct.FromGlobalMiddleWeight + aggression,0.0f,1.0f);
	newStruct.FromLocalMiddleWeight = FMath::Clamp(newStruct.FromGlobalMiddleWeight + aggression,0.0f,1.0f);

	//music feel
	newStruct.VortexWeight = FMath::Clamp(newStruct.VortexWeight + musicFeel,0.0f,1.0f);	
	newStruct.ToGlobalMiddleWeight = FMath::Clamp(newStruct.ToGlobalMiddleWeight + musicFeel,0.0f,1.0f);

	//group tihnk
	newStruct.ToLocalMiddleWeight = FMath::Clamp(newStruct.ToLocalMiddleWeight + groupThink,0.0f,1.0f);


	
	return newStruct;
}




/*
// Called every frame
void UMosherStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/


void UMosherStateMachine::StateTransition(EMosherState newState)
{

	mosherState = newState;
}


EMosherState UMosherStateMachine::GetMostCommonState(TArray<AActor*> neighbours)
{


	int moshCount = 0;
	int thrashCount = 0;
	int vortexCount = 0;
	for (AActor* neighbour : neighbours)
	{
		AMosherBase* moshBase = Cast<AMosherBase>(neighbour);

		if(moshBase->GetMosherState() == EMosherState::Moshing)
			moshCount++;
		else if(moshBase->GetMosherState() == EMosherState::Vortexing)
			vortexCount++;
		else if(moshBase->GetMosherState()== EMosherState::Thrashing)
			thrashCount++;
	}
    
    if(moshCount > thrashCount && moshCount > vortexCount)
    	return EMosherState::Moshing;
	else if(thrashCount > moshCount && thrashCount > vortexCount)
		return EMosherState::Thrashing;
	else if(vortexCount > moshCount && vortexCount > thrashCount)
		return EMosherState::Vortexing;
	else
	{
		int randInt = FMath::RandRange(0,2);
		if(randInt == 0)
			return EMosherState::Moshing;
		if(randInt == 1)
			return EMosherState::Thrashing;
		if(randInt == 2)
			return EMosherState::Vortexing;

		return EMosherState::Moshing;

		
	}
	
	
}




