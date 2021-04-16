// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <AudioClient.h>


#include "SimManager.h"
#include "Chaos/AABBTree.h"
#include "Components/ActorComponent.h"
#include "MosherStateMachine.generated.h"

//component which handles the state transitions/values of the mosher
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CROWDSIM_API UMosherStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMosherStateMachine();


	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMosherState> mosherState = EMosherState::Idle;


protected:

	FMosherStruct* mosherStateStruct;



	//AMosherBase* MosherObj;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void StateTransition(EMosherState newState);
	
	UFUNCTION(BlueprintCallable)
	FMosherStruct UpdateState(TArray<AActor*>neighbours,ASimManager* SimManager,float musicIntensity,
		float aggression, float musicFeel, float groupThink);

	EMosherState GetMostCommonState(TArray<AActor*>);
		
};
