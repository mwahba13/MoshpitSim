// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "FindInBlueprintManager.h"
//#include "MosherBase.h"
#include "GameFramework/Actor.h"
#include "SimManager.generated.h"

USTRUCT(BlueprintType)
struct CROWDSIM_API FMosherStruct
{
	GENERATED_BODY()

	
	//Gaseous force is the random movement representing gas-like state
	UPROPERTY(EditAnywhere,Category="Gaslike Parameters")
	float GasLikeWeight;
	UPROPERTY(EditAnywhere,Category="Gaslike Parameters")
	float GasLikeForce;

	//vortexing around the center of the crowd
	UPROPERTY(EditAnywhere,Category="Vortexing Parameters")
	float VortexWeight;
	UPROPERTY(EditAnywhere,Category="Vortexing Parameters")
	float VortexForce;

	//Force towards neighbours
	UPROPERTY(EditAnywhere,Category="Thrashing Parameters")
	float ThrashWeight;
	UPROPERTY(EditAnywhere,Category="Thrashing Parameters")
	float ThrashForce;
	

	//force directed towards global middle
	UPROPERTY(EditAnywhere,Category="Towards Global Mid Parameters")
	float ToGlobalMiddleWeight;
	UPROPERTY(EditAnywhere,Category="Towards Global Mid Parameters")
	float ToGlobalMiddleForce;

	//force directed away from global middle
	UPROPERTY(EditAnywhere,Category="Away Global Mid Parameters")
	float FromGlobalMiddleWeight;
	UPROPERTY(EditAnywhere,Category="Away Global Mid Parameters")
	float FromGlobalMiddleForce;
	
	//force directed towards local middle
	UPROPERTY(EditAnywhere, Category = "Towards Local Mid Parameters")
	float ToLocalMiddleWeight;
	UPROPERTY(EditAnywhere, Category="Towards Local Mid Parameters")
	float ToLocalMiddleForce;

	//force directed away from local middle
	UPROPERTY(EditAnywhere,Category="Away Local Mid Parameters")
	float FromLocalMiddleWeight;
	UPROPERTY(EditAnywhere,Category="Away Local Mid Parameters")
	float FromLocalMiddleForce;

};

UENUM(Blueprintable)
enum EMosherState { Idle,Moshing,Vortexing, Thrashing  };

UCLASS()
class CROWDSIM_API ASimManager : public AActor
{
	GENERATED_BODY()

	//this class contains the constant values for the sim
	
public:	
	// Sets default values for this actor's properties
	ASimManager();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Global Settings")
	TEnumAsByte<EMosherState> GlobalState;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Global Settings")
	USoundWave* CurrentSong;

	UPROPERTY(EditAnywhere,Category="Mosher States")
	FMosherStruct IdleStateStruct;

	UPROPERTY(EditAnywhere,Category="Mosher States")
	FMosherStruct MoshStateStruct;

	UPROPERTY(EditAnywhere,Category="Mosher States")
	FMosherStruct VortexStateStruct;

	UPROPERTY(EditAnywhere,Category="Mosher States")
	FMosherStruct ThrashingStateStruct;
	
	//mean and SD to generate normally distributed toughness values
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mosher Traits")
	FVector2D aggression;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mosher Traits")
	FVector2D musicFeel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mosher Traits")
	FVector2D groupThink;

	UPROPERTY(EditAnywhere,Category="Mosher Settings")
	float maxSteeringForce;

	UPROPERTY(EditAnywhere,Category="Mosher Settings")
	float moshRedirectTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Mosher Settings")
	float musicIntensity;

	UPROPERTY(EditAnywhere)
	UMaterial* MoshMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* VortexMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* ThrashMaterial;
	
	//Shows Gas-like force vector in red
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Debug")
	bool DebugLineGaseous;

	//Shows vortexing force vector in blue
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Debug")
	bool DebugLineVortex;
	
	//Shows final calculated vector in grey
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Debug")
	bool DebugLineFinalVector;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Debug")
	bool DebugShowNeighbourHoods;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FMosherStruct* GetStateStructPtr(EMosherState);
	FMosherStruct GetStateStruct(EMosherState);

	UFUNCTION(BlueprintCallable)
	float GenerateGaussianFloat(float x, float std);

	UFUNCTION(BlueprintCallable)
	void SetTraitVectors(FVector2D aggro, FVector2D music,FVector2D group);
};
