// Fill out your copyright notice in the Description page of Project Settings.


#include "FileUtilityLibrary.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"


void UFileUtilityLibrary::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames)
{
	if(GEngine)
	{
		if(GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if(DesktopPlatform)
			{
				uint32 selectionFlag = 0;
				DesktopPlatform->OpenFileDialog(ParentWindowHandle,DialogTitle,DefaultPath,FString(""),
					FileTypes,selectionFlag,OutFileNames);
			}
		}
	}
}





