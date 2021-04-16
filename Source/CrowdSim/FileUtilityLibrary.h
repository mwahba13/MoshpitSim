// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FileUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIM_API UFileUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, Category="File Utility")
	static void OpenFileDialog(const FString& DialogTitle,const FString& DefaultPath,
		const FString& FileTypes,TArray<FString>& OutFileNames);


};
